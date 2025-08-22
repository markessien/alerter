#include "telex.h"
#include "httplib.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/* Testing in powershell

Invoke-WebRequest -Uri "https://api.staging.telex.im/api/v1/auth/login" `
                  -Method POST `
                  -ContentType "application/json" `
                  -Body '{"email":"markessien@gmail.com", "password":"123456789"}'

*/

Telex::Telex() {
    base_url = "https://api.staging.telex.im";
}

bool Telex::login(const std::string& email, const std::string& password) {
    httplib::Client cli(base_url);

    // cli.enable_server_certificate_verification(false);

    json request_body;
    request_body["email"] = email;
    request_body["password"] = password;

    auto res = cli.Post("/api/v1/auth/login", request_body.dump(), "application/json");
    if (!res) {
        auto err = res.error();
        std::cerr << "Request failed. Error: " << httplib::to_string(err) << std::endl;
        return false;
    }

    if (res && res->status == 200) {
        // Login successful
        return true;
    } else {
        // Login failed
        std::cerr << "Login failed. Status: " << res->status << std::endl;
        std::cerr << "Response body: " << res->body << std::endl;
        return false;
    }
}