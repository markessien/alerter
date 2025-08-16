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
    // Clean up previous pipe instance if it exists
    if (_pipe != null) {
      await _pipe!.close();
      _pipe = null;
    }
    _isPipeOpen = false;

    try {
      _log('Initializing pipe...');
      var file = File(_pipeName);
      _pipe = await file.open(mode: FileMode.writeOnlyAppend);
      _isPipeOpen = true;
      _log('Pipe opened successfully.');
    } on FileSystemException {
      _log('Pipe $_pipeName does not exist. Waiting for it to be created.');
    } catch (e) {
      _log('Error initializing pipe: $e');
    }
  }

  Future<void> send({
    required String message,
    required String senderName,
    required String channel,
    required String iconPath,
    required String type,
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
      'timestamp': DateTime.now().toIso8601String(),
      'type': type,
    };

    final jsonString = jsonEncode(notificationData);
    final bytes = utf8.encode(jsonString + '\n');

    try {
      _log('Sending data: $jsonString');
      await _pipe!.writeFrom(bytes);
      await _pipe!.flush();
      _log('Data sent successfully.');
    } on FileSystemException catch (e) {
      // Error code 233: No process is on the other end of the pipe.
      if (e.osError?.errorCode == 233) {
        _log('Notification service not running. Could not send notification.');
      } else {
        _log('Error writing to pipe: $e');
      }
      _isPipeOpen = false;
      await _pipe?.close();
      _pipe = null;
    } catch (e) {
      _log('Unexpected error writing to pipe: $e');
      _isPipeOpen = false;
      await _pipe?.close();
      _pipe = null;
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
