import 'package:alerter_test_app/http_client.dart';

class Notifications {
  final Function(String)? _logger;
  late final HttpClient _httpClient;

  Notifications({Function(String)? logger}) : _logger = logger {
    _httpClient = HttpClient(logger: _log);
  }

  Future<void> send(Map<String, dynamic> jsonData) async {
    await _httpClient.send(jsonData);
  }

  void _log(String message) {
    _logger?.call(message);
  }
}
