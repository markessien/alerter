import 'package:alerter_test_app/pipe_reader.dart';
import 'package:alerter_test_app/pipe_writer.dart';

class Notifications {
  final Function(String)? _logger;
  late final PipeWriter _pipeWriter;
  late final PipeReader _pipeReader;

  Notifications({Function(String)? logger}) : _logger = logger {
    _pipeWriter = PipeWriter(logger: _log);
    _pipeReader = PipeReader(logger: _log, onMessage: _handleMessage);
  }

  Future<void> send(Map<String, dynamic> jsonData) async {
    await _pipeWriter.send(jsonData);
  }

  void _handleMessage(Map<String, dynamic> message) {
    _log('Received message: $message');
  }

  Future<void> close() async {
    await _pipeWriter.close();
    await _pipeReader.close();
  }

  void _log(String message) {
    _logger?.call(message);
  }
}
