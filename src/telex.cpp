#include "telex.h"
#include "httplib.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Telex::Telex() : base_url("https://api.staging.telex.im/") {
    // Constructor implementation
}

bool Telex::login(const std::string& email, const std::string& password) {
    httplib::Client cli(base_url);

    json request_body;
    request_body["email"] = email;
    request_body["password"] = password;

    auto res = cli.Post("/auth/login", request_body.dump(), "application/json");

    if (res && res->status == 200) {
        // Login successful
        return true;
    } else {
        // Login failed
        return false;
    }
}