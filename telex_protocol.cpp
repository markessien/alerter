#include "telex_protocol.h"
#include <algorithm>
#include <cctype>

namespace TelexProtocol {

std::string FrameMessage(const json& payload) {
    std::string payload_str = payload.dump();
    std::string len_str = std::to_string(payload_str.length());
    len_str.insert(0, LENGTH_DIGITS - len_str.length(), '0');
    return START_MARKER + "[" + len_str + "]+" + payload_str;
}

bool TryParseMessage(std::string& buffer, std::string& json_payload) {
    auto start_pos = buffer.find(START_MARKER);
    if (start_pos == std::string::npos) {
        buffer.clear();
        return false;
    }
    if (start_pos > 0) {
        buffer.erase(0, start_pos);
    }

    if (buffer.length() < FULL_HEADER_LEN) {
        return false; // Incomplete header
    }

    size_t len_start_pos = START_MARKER.length() + 1;
    std::string len_str = buffer.substr(len_start_pos, LENGTH_DIGITS);

    if (buffer[START_MARKER.length()] != '[' ||
        buffer.substr(len_start_pos + LENGTH_DIGITS, END_MARKER.length()) != END_MARKER ||
        !std::all_of(len_str.begin(), len_str.end(), ::isdigit)) {
        // Malformed header, discard the marker and rescan
        buffer.erase(0, START_MARKER.length());
        return false;
    }

    size_t length = std::stoul(len_str);
    size_t total_message_size = FULL_HEADER_LEN + length;

    if (buffer.length() < total_message_size) {
        return false; // Incomplete payload
    }

    json_payload = buffer.substr(FULL_HEADER_LEN, length);
    buffer.erase(0, total_message_size);
    return true;
}

} // namespace TelexProtocol