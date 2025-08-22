#include "telex.h"
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;


Telex::Telex() {
    base_url = "https://api.staging.telex.im";
}

bool Telex::login(const std::string& email, const std::string& password) {
    httplib::Client cli(base_url);

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
        json response_body = json::parse(res->body);
        if (response_body.contains("data")) {
            auto& data = response_body["data"];
            if (data.contains("notification_token")) {
                notification_token_ = data["notification_token"];
            }
            if (data.contains("access_token")) {
                access_token_ = data["access_token"];
            }
            if (data.contains("user") && data["user"].contains("id")) {
                user_id_ = data["user"]["id"];
            }
            testTelex();
            return true;
        }
    }

    // Login failed
    std::cerr << "Login failed. Status: " << res->status << std::endl;
    std::cerr << "Response body: " << res->body << std::endl;
    return false;
}

std::string Telex::getConnectionToken() {
    httplib::Client cli(base_url);
    httplib::Headers headers = {
        {"X-Notification-Token", notification_token_}
    };

    auto res = cli.Get("/api/v1/centrifugo/connection", headers);
    if (res && res->status == 200) {
        json response_body = json::parse(res->body);
        if (response_body.contains("data") && response_body["data"].contains("token")) {
            return response_body["data"]["token"];
        }
    }
    return "";
}

void Telex::testTelex() {
    std::cout << "Login successful" << std::endl;
    std::string connectionToken = getConnectionToken();
    if (!connectionToken.empty()) {
        std::cout << "Connection Token: " << connectionToken << std::endl;
    } else {
        std::cerr << "Failed to get connection token" << std::endl;
    }

    auto organisations = getUserOrganisations();
    for (const auto& org : organisations) {
        std::string channel = org.id + "/" + user_id_;
        std::string subscriptionToken = getSubscriptionToken(channel);
        if (!subscriptionToken.empty()) {
            std::cout << "Subscription Token for " << org.name << ": " << subscriptionToken << std::endl;
        } else {
            std::cerr << "Failed to get subscription token for " << org.name << std::endl;
        }
    }
}

std::string Telex::getSubscriptionToken(const std::string& channel) {
    httplib::Client cli(base_url);
    httplib::Headers headers = {
        {"X-Notification-Token", notification_token_}
    };

    json request_body;
    request_body["channel"] = channel;

    auto res = cli.Post("/api/v1/centrifugo/subscription", headers, request_body.dump(), "application/json");
    if (res && res->status == 200) {
        json response_body = json::parse(res->body);
        if (response_body.contains("data") && response_body["data"].contains("token")) {
            return response_body["data"]["token"];
        }
    }
    return "";
}

std::vector<Organisation> Telex::getUserOrganisations() {
    httplib::Client cli(base_url);
    httplib::Headers headers = {
        {"Authorization", "Bearer " + access_token_}
    };

    auto res = cli.Get("/api/v1/users/organisations", headers);
    if (res && res->status == 200) {
        std::cout << "Organisations response: " << res->body << std::endl;
        json response_body = json::parse(res->body);
        std::vector<Organisation> orgs;
        if (response_body.contains("data") && response_body["data"].is_array()) {
            for (const auto& org_json : response_body["data"]) {
                if (org_json.contains("id") && org_json.contains("name")) {
                    orgs.push_back({org_json["id"], org_json["name"]});
                }
            }
        }
        return orgs;
    }
    return {};
}