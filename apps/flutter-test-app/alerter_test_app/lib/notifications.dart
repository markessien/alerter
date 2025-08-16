import 'dart:io';
import 'dart:convert';

class Notifications {
  final String _pipeName = r'\\.\pipe\telex-notifications';
  RandomAccessFile? _pipe;
  bool _isPipeOpen = false;

  Notifications() {
    _initPipe();
  }

  Future<void> _initPipe() async {
    try {
      var file = File(_pipeName);
      // The other process is responsible for creating the pipe.
      // This class is a client that writes to it.
      if (await file.exists()) {
        _pipe = await file.open(mode: FileMode.writeOnlyAppend);
        _isPipeOpen = true;
      } else {
        print('Pipe $_pipeName does not exist. Waiting for it to be created.');
      }
    } catch (e) {
      print('Error opening pipe: $e');
      _isPipeOpen = false;
    }
  }

  Future<void> send({
    required String message,
    required String senderName,
    required String channel,
    required String iconPath,
  }) async {
    if (!_isPipeOpen || _pipe == null) {
      // Try to reopen the pipe
      await _initPipe();
      if (!_isPipeOpen || _pipe == null) {
        print('Cannot send notification, pipe is not open.');
        return;
      }
    }

    final notificationData = {
      'message': message,
      'senderName': senderName,
      'channel': channel,
      'iconPath': iconPath,
    };

    final jsonString = jsonEncode(notificationData);
    final bytes = utf8.encode(jsonString + '\n');

    try {
      await _pipe!.writeFrom(bytes);
      await _pipe!.flush();
    } catch (e) {
      print('Error writing to pipe: $e');
      _isPipeOpen = false; // Pipe might be broken
    }
  }

  Future<void> close() async {
    if (_isPipeOpen) {
      await _pipe?.close();
      _isPipeOpen = false;
    }
  }
}
