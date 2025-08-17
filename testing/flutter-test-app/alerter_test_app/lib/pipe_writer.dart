import 'dart:io';
import 'dart:async';
import 'dart:convert';
import 'package:alerter_test_app/telex_protocol.dart';

class PipeWriter {
  final String _pipeName = r'\\?\pipe\telex-notifications-c2s1';
  RandomAccessFile? _pipe;
  bool _isPipeOpen = false;
  final Function(String)? _logger;

  PipeWriter({Function(String)? logger}) : _logger = logger;

  Future<void> _connect() async {
    if (_isPipeOpen) return;
    try {
      var file = File(_pipeName);
      _pipe = await file.open(mode: FileMode.writeOnlyAppend);
      _isPipeOpen = true;
      _log('C2S pipe opened successfully!');
    } catch (e) {
      _log('Failed to connect to C2S pipe: ${e.toString()}');
      // await close();
    }
  }

  Future<void> send(Map<String, dynamic> jsonData) async {
    if (!_isPipeOpen || _pipe == null) {
      await _connect();
    }

    if (!_isPipeOpen || _pipe == null) {
      _log('Pipe not available. Dropping message.');
      return;
    }
    try {
      final message = TelexProtocol.encode(jsonData);
      await _pipe!.writeFrom(utf8.encode(message));
      await _pipe!.flush();
    } catch (e) {
      _log('Error writing to C2S pipe: $e.');
      // await close();
    }
  }

  Future<void> close() async {
    if (_pipe != null) {
      await _pipe?.close();
      _pipe = null;
    }
    _isPipeOpen = false;
  }

  void _log(String message) {
    _logger?.call(message);
  }
}
