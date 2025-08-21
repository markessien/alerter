#include "http_server.h"
#include "httplib.h"
#include "telex_protocol.h"
#include "notifications.h"
#include <nlohmann/json.hpp>
#include <wx/wx.h>
#include "messaging.h"
#include <wx/datetime.h>

using json = nlohmann::json;

static httplib::Server svr;

void run_http_server(wxEvtHandler* parent) {
    svr.Post("/", [parent](const httplib::Request &req, httplib::Response &res) {
        std::string body = req.body;
        std::string json_payload;
        if (TelexProtocol::TryParseMessage(body, json_payload)) {
            try {
                json j = json::parse(json_payload);
                wxVector<wxString> payload;
                payload.push_back(wxString::FromUTF8(j.value("message", "")));
                payload.push_back(wxString::FromUTF8(j.value("senderName", "")));
                payload.push_back(wxString::FromUTF8(j.value("channel", "")));
                payload.push_back(wxString::FromUTF8(j.value("iconPath", "")));
                wxString timestampStr = wxString::FromUTF8(j.value("timestamp", ""));
                wxDateTime timestamp;
                if (!timestampStr.IsEmpty() && timestamp.ParseISOCombined(timestampStr, ' ')) {
                    // Successfully parsed
                } else {
                    timestamp = wxDateTime::Now();
                }
                payload.push_back(timestamp.FormatISOCombined('T'));
                payload.push_back(wxString::FromUTF8(j.value("type", "")));

                wxThreadEvent* event = new wxThreadEvent(wxEVT_COMMAND_MYTHREAD_NOTIFICATION);
                event->SetPayload(payload);
                wxQueueEvent(parent, event);
                res.set_content("OK", "text/plain");
            } catch (const json::parse_error& e) {
                res.status = 400;
                res.set_content("Invalid JSON payload", "text/plain");
            }
        } else {
            res.status = 400;
            res.set_content("Invalid Telex message", "text/plain");
        }
    });

    svr.listen("127.0.0.1", 27810);
}

void stop_http_server() {
    if (svr.is_running()) {
        svr.stop();
    }
}