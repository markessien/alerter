#include <winsock2.h>
#include "reader_thread.h"
#include "messaging.h"
#include "logger.h"
#include "telex_protocol.h"
#include "include/json.hpp"
#include "pipe.h"

using json = nlohmann::json;

ReaderThread::ReaderThread(wxEvtHandler* parent, wxMessageQueue<std::string>& queue)
    : wxThread(wxTHREAD_JOINABLE), m_pParent(parent), m_responseQueue(queue) {}

wxThread::ExitCode ReaderThread::Entry() {
    Pipe c2sPipe(L"\\\\.\\pipe\\telex-notifications-c2s1", PIPE_ACCESS_INBOUND, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT);
    if (!c2sPipe.IsValid()) {
        return (wxThread::ExitCode)1;
    }

    std::string buffer;
    char read_chunk[4096];

    while (!TestDestroy()) {
        if (c2sPipe.Connect()) {
            buffer.clear();

            while (!TestDestroy()) {
                DWORD dwRead;
                if (!c2sPipe.Read(read_chunk, sizeof(read_chunk), dwRead) || dwRead == 0) {
                    break; // Client disconnected
                }
                buffer.append(read_chunk, dwRead);

                std::string json_payload;
                while (TelexProtocol::TryParseMessage(buffer, json_payload)) {
                    try {
                        json j = json::parse(json_payload);
                        wxVector<wxString> payload;
                        payload.push_back(j["message"].get<std::string>());
                        payload.push_back(j["senderName"].get<std::string>());
                        payload.push_back(j["channel"].get<std::string>());
                        payload.push_back(j["iconPath"].get<std::string>());
                        payload.push_back(j["timestamp"].get<std::string>());
                        payload.push_back(j["type"].get<std::string>());

                        wxThreadEvent* event = new wxThreadEvent(wxEVT_COMMAND_MYTHREAD_NOTIFICATION);
                        event->SetPayload(payload);
                        wxQueueEvent(m_pParent, event);
                        m_responseQueue.Post("ok:Message processed successfully.");
                    } catch (const json::exception& e) {
                        m_responseQueue.Post("error:JSON was malformed.");
                        LogMessage(wxString::Format("JSON parse error: %s", e.what()));
                    }
                }
            }
            c2sPipe.Disconnect();
        }
    }
    return (wxThread::ExitCode)0;
}