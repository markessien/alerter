import 'dart:async';
import 'dart:convert';
import 'package:alerter_test_app/telex_protocol.dart';
import 'package:http/http.dart' as http;

class HttpClient {
  final String _url = 'http://127.0.0.1:27810/';
  final Function(String)? _logger;

  HttpClient({Function(String)? logger}) : _logger = logger;

  Future<void> send(Map<String, dynamic> jsonData) async {
    try {
      final message = TelexProtocol.encode(jsonData);
      final response = await http.post(
        Uri.parse(_url),
        headers: {'Content-Type': 'text/plain'},
        body: message,
      );

      if (response.statusCode != 200) {
        _log('Error sending message: ${response.statusCode}');
      }
    } catch (e) {
      _log('Error sending message: $e');
    }
  }

  void _log(String message) {
    _logger?.call(message);
  }
}
