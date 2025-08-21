#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace TelexProtocol {
    // Protocol constants
    const std::string START_MARKER = "+TELEX+:";
    const std::string END_MARKER = "]+";
    constexpr size_t LENGTH_DIGITS = 8;
    const size_t FULL_HEADER_LEN = START_MARKER.length() + 1 + LENGTH_DIGITS + END_MARKER.length();

    // Functions
    std::string FrameMessage(const json& payload);
    bool TryParseMessage(std::string& buffer, std::string& json_payload);
}