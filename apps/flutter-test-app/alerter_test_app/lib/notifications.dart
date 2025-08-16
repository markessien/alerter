import 'dart:io';
import 'dart:convert';

class Notifications {
  final String _pipeName = r'\\?\pipe\telex-notifications';
  RandomAccessFile? _pipe;
  bool _isPipeOpen = false;
  final Function(String)? _logger;

  Notifications({Function(String)? logger}) : _logger = logger {
    _initPipe();
  }

  Future<void> _initPipe() async {
    try {
      _log('Initializing pipe...');
      var file = File(_pipeName);
      // The other process is responsible for creating the pipe.
      // This class is a client that writes to it.
      try {
        _pipe = await file.open(mode: FileMode.writeOnlyAppend);
        _isPipeOpen = true;
        _log('Pipe opened successfully.');
      } on FileSystemException {
        _log('Pipe $_pipeName does not exist. Waiting for it to be created.');
      }
    } catch (e) {
      _log('Error initializing pipe: $e');
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
      _log('Pipe not open. Attempting to reconnect...');
      await _initPipe();
      if (!_isPipeOpen || _pipe == null) {
        _log('Cannot send notification, pipe is not open.');
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
      _log('Sending data: $jsonString');
      await _pipe!.writeFrom(bytes);
      await _pipe!.flush();
      _log('Data sent successfully.');
    } catch (e) {
      _log('Error writing to pipe: $e');
      _isPipeOpen = false; // Pipe might be broken
    }
  }

  Future<void> close() async {
    if (_isPipeOpen) {
      await _pipe?.close();
      _isPipeOpen = false;
      _log('Pipe closed.');
    }
  }

  void _log(String message) {
    _logger?.call(message);
  }
}
