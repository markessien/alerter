#include "telex.h"
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "events.h"

using json = nlohmann::json;

wxDEFINE_EVENT(wxEVT_LOGIN_REQUEST, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_LOGIN_SUCCESS, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_LOGIN_FAILURE, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_WEBSOCKET_ERROR, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_SHOW_NOTIFICATION, wxCommandEvent);

Telex::Telex(wxEvtHandler* pParent) : wxThread(wxTHREAD_JOINABLE), m_pParent(pParent), m_queue(pParent) {
    base_url = "https://api.staging.telex.im";
}

Telex::~Telex() {
    webSocket_.stop();
}

void Telex::login(const std::string& email, const std::string& password) {
    Job job;
    job.command = CMD_LOGIN;
    job.string_arg1 = email;
    job.string_arg2 = password;
    m_queue.AddJob(job);
}

void Telex::Exit()
{
    Job job;
    job.command = CMD_EXIT;
    m_queue.AddJob(job);
}

std::vector<Organisation> Telex::getOrganisations() {
    return organisations_;
}

wxThread::ExitCode Telex::Entry() {
    while (!TestDestroy()) {
        Job job = m_queue.Pop();
        if (TestDestroy()) break;

        switch (job.command) {
            case CMD_LOGIN:
            {
                if (doLogin(job.string_arg1.ToStdString(), job.string_arg2.ToStdString())) {
                    fetchUserOrganisations();
                    std::string connectionToken = getConnectionToken();
                    if (!connectionToken.empty()) {
                        std::map<std::string, std::string> subscriptions;
                        for (const auto& org : organisations_) {
                            std::string channel = org.id + "/" + user_id_;
                            std::string subscriptionToken = getSubscriptionToken(channel);
                            if (!subscriptionToken.empty()) {
                                subscriptions[channel] = subscriptionToken;
                            }
                        }
                        if (!subscriptions.empty()) {
                            connectAndSubscribeViaWebSocket(connectionToken, subscriptions);
                            wxCommandEvent event(wxEVT_LOGIN_SUCCESS);
                            m_queue.Report(event);
                        } else {
                            wxCommandEvent event(wxEVT_LOGIN_FAILURE);
                            m_queue.Report(event);
                        }
                    } else {
                        wxCommandEvent event(wxEVT_LOGIN_FAILURE);
                        m_queue.Report(event);
                    }
                } else {
                    wxCommandEvent event(wxEVT_LOGIN_FAILURE);
                    m_queue.Report(event);
                }
                break;
            }
            case CMD_EXIT:
            {
                return 0;
            }
        }
    }
    return 0;
}

bool Telex::doLogin(const std::string& email, const std::string& password) {
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
            return true;
        }
    }

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

void Telex::connectAndSubscribeViaWebSocket(
    const std::string& connectionToken,
    const std::map<std::string, std::string>& subscriptions
) {
    std::string ws_url = "wss://api.telex.im/centrifugo/connection/websocket";
    webSocket_.setUrl(ws_url);

    webSocket_.setOnMessageCallback(
        [this, connectionToken, subscriptions](const ix::WebSocketMessagePtr& msg) {
            if (msg->type == ix::WebSocketMessageType::Open) {
                json connect_params;
                connect_params["token"] = connectionToken;

                json subs_obj;
                for (const auto& pair : subscriptions) {
                    const std::string& channel = pair.first;
                    const std::string& subToken = pair.second;
                    subs_obj[channel] = { {"token", subToken} };
                }
                if (!subs_obj.empty()) {
                    connect_params["subs"] = subs_obj;
                }

                json command;
                command["id"] = command_id_++;
                command["connect"] = connect_params;
                webSocket_.send(command.dump());

            } else if (msg->type == ix::WebSocketMessageType::Message) {
                if (msg->str == "{}") {
                    webSocket_.send("{}");
                    return;
                }
                
                try {
                    json data = json::parse(msg->str);
                    if (data.contains("push") && data["push"].contains("pub") && data["push"]["pub"].contains("data")) {
                        auto& pub_data = data["push"]["pub"]["data"];
                        if (pub_data.contains("notification_type") && pub_data["notification_type"] == "unread_thread_change") {
                            if (pub_data.contains("data")) {
                                auto& notification_data = pub_data["data"];
                                wxCommandEvent* event = new wxCommandEvent(wxEVT_SHOW_NOTIFICATION);
                                event->SetString(notification_data["name"].get<std::string>());
                                event->SetClientData(new wxStringClientData(notification_data["description"].get<std::string>()));
                                wxQueueEvent(m_pParent, event);
                            }
                        }
                    }
                } catch (const json::parse_error& e) {
                    std::cerr << "JSON parse error: " << e.what() << std::endl;
                }

            } else if (msg->type == ix::WebSocketMessageType::Error) {
                wxCommandEvent* event = new wxCommandEvent(wxEVT_WEBSOCKET_ERROR);
                event->SetString(msg->errorInfo.reason);
                wxQueueEvent(m_pParent, event);
            } else if (msg->type == ix::WebSocketMessageType::Close) {
                std::cout << "WebSocket connection closed. Code: " << msg->closeInfo.code
                          << ", Reason: " << msg->closeInfo.reason << std::endl;
            }
        }
    );

    webSocket_.start();
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

void Telex::fetchUserOrganisations() {
    httplib::Client cli(base_url);
    httplib::Headers headers = {
        {"Authorization", "Bearer " + access_token_}
    };

    auto res = cli.Get("/api/v1/users/organisations", headers);
    if (res && res->status == 200) {
        json response_body = json::parse(res->body);
        organisations_.clear();
        if (response_body.contains("data") && response_body["data"].is_array()) {
            for (const auto& org_json : response_body["data"]) {
                if (org_json.contains("id") && org_json.contains("name")) {
                    organisations_.push_back({org_json["id"], org_json["name"]});
                }
            }
        }
    }
}