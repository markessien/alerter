import 'dart:convert';
import 'dart:typed_data';

class TelexProtocol {
  static String encode(Map<String, dynamic> jsonData) {
    String jsonPayload = jsonEncode(jsonData);
    int payloadLength = utf8.encode(jsonPayload).length;
    String lengthString = payloadLength.toString().padLeft(8, '0');
    return "+TELEX+:[$lengthString]+$jsonPayload";
  }

  static List<Map<String, dynamic>> decode(BytesBuilder buffer) {
    final List<Map<String, dynamic>> messages = [];
    final List<int> startMarker = utf8.encode('+TELEX+:');
    const int lengthDigits = 8;
    const int fullHeaderLen = 19;

    Uint8List currentBuffer = buffer.toBytes();

    while (currentBuffer.isNotEmpty) {
      int startPos = _indexOf(currentBuffer, startMarker);
      if (startPos == -1) {
        break;
      }

      if (startPos > 0) {
        currentBuffer = currentBuffer.sublist(startPos);
      }

      if (currentBuffer.length < fullHeaderLen) {
        break;
      }

      final int lenStartPos = startMarker.length + 1;
      final int lenEndPos = lenStartPos + lengthDigits;
      final int endMarkerPos = lenEndPos + 1;

      String lenStr = utf8.decode(
        currentBuffer.sublist(lenStartPos, lenEndPos),
        allowMalformed: true,
      );

      if (currentBuffer[startMarker.length] != 91 /* [ */ ||
          currentBuffer[endMarkerPos] != 93 /* ] */ ||
          currentBuffer[endMarkerPos + 1] != 43 /* + */ ||
          int.tryParse(lenStr) == null) {
        currentBuffer = currentBuffer.sublist(startMarker.length);
        continue;
      }

      int jsonLength = int.parse(lenStr);
      int totalMessageSize = fullHeaderLen + jsonLength;

      if (currentBuffer.length < totalMessageSize) {
        break;
      }

      Uint8List jsonPayloadBytes = currentBuffer.sublist(
        fullHeaderLen,
        totalMessageSize,
      );
      String jsonPayload = utf8.decode(jsonPayloadBytes);

      try {
        var decodedJson = jsonDecode(jsonPayload);
        messages.add(decodedJson);
      } catch (e) {
        // Ignore JSON parse errors
      }

      currentBuffer = currentBuffer.sublist(totalMessageSize);
    }

    buffer.clear();
    buffer.add(currentBuffer);

    return messages;
  }

  static int _indexOf(Uint8List source, List<int> target) {
    if (target.isEmpty) return 0;
    for (int i = 0; i <= source.length - target.length; i++) {
      bool found = true;
      for (int j = 0; j < target.length; j++) {
        if (source[i + j] != target[j]) {
          found = false;
          break;
        }
      }
      if (found) return i;
    }
    return -1;
  }
}
