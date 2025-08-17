import 'dart:io';
import 'dart:async';
import 'dart:typed_data';
import 'package:alerter_test_app/telex_protocol.dart';

class PipeReader {
  final String _pipeName = r'\\?\pipe\telex-notifications-s2c';
  RandomAccessFile? _pipe;
  bool _isPipeOpen = false;
  final Function(String)? _logger;
  final Function(Map<String, dynamic>)? _onMessage;
  final BytesBuilder _buffer = BytesBuilder();

  PipeReader({
    Function(String)? logger,
    Function(Map<String, dynamic>)? onMessage,
  }) : _logger = logger,
       _onMessage = onMessage {
    _connect();
  }

  Future<void> _connect() async {
    return;
    if (_isPipeOpen) return;

    try {
      var file = File(_pipeName);
      _pipe = await file.open(mode: FileMode.read);
      _isPipeOpen = true;
      _log('S2C pipe opened successfully!');
      _listen();
    } on FileSystemException {
      _log('S2C pipe not available on initial connection.');
    } catch (e) {
      _log('Error opening S2C pipe: $e');
    }
  }

  void _listen() {
    return;
    Future(() async {
      try {
        while (_isPipeOpen) {
          final buffer = Uint8List(1024);
          final bytesRead = await _pipe!.readInto(buffer);
          if (bytesRead > 0) {
            _buffer.add(buffer.sublist(0, bytesRead));
            final messages = TelexProtocol.decode(_buffer);
            for (final message in messages) {
              _onMessage?.call(message);
            }
          } else {
            break;
          }
        }
      } catch (e) {
        if (_isPipeOpen) {
          _log('Pipe read error: $e');
        }
      } finally {
        await close();
      }
    });
  }

  Future<void> close() async {
    if (_isPipeOpen) {
      await _pipe?.close();
      _pipe = null;
      _isPipeOpen = false;
      _log('S2C pipe closed.');
    }
  }

  void _log(String message) {
    _logger?.call(message);
  }
}
