#include <wx/wx.h>
#include "messaging.h"
#include "include/json.hpp"

using json = nlohmann::json;

wxDEFINE_EVENT(wxEVT_COMMAND_MYTHREAD_NOTIFICATION, wxThreadEvent);

Messaging::Messaging(wxEvtHandler* pParent) : wxThread(wxTHREAD_DETACHED), m_pParent(pParent) {}

wxThread::ExitCode Messaging::Entry() {
    // Allow the main app to initialize
    wxSleep(2);

    HANDLE hPipe = CreateNamedPipe(
        TEXT("\\\\.\\pipe\\telex-notifications"),
        PIPE_ACCESS_INBOUND,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1, // nMaxInstances
        1024 * 16, // nOutBufferSize
        1024 * 16, // nInBufferSize
        NMPWAIT_USE_DEFAULT_WAIT, // nDefaultTimeOut
        NULL // lpSecurityAttributes
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        wxMessageBox("Failed to create named pipe", "Error", wxOK | wxICON_ERROR);
        return (wxThread::ExitCode)0;
    }

    while (!TestDestroy()) {
        // Wait for the client to connect; if it succeeds,
        // the function returns a nonzero value. If the function
        // returns zero, GetLastError returns ERROR_PIPE_CONNECTED.
        bool connected = ConnectNamedPipe(hPipe, NULL) ?
            true : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (connected) {
            char buffer[1024];
            DWORD dwRead;
            // Read client requests from the pipe.
            if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL)) {
                buffer[dwRead] = '\0';
                std::string stdString = std::string(buffer);

                try {
                    json j = json::parse(stdString);

                    wxString message = j["message"].get<std::string>();
                    wxString senderName = j["senderName"].get<std::string>();
                    wxString channel = j["channel"].get<std::string>();
                    wxString iconPath = j["iconPath"].get<std::string>();

                    wxVector<wxString> payload;
                    payload.push_back(message);
                    payload.push_back(senderName);
                    payload.push_back(channel);
                    payload.push_back(iconPath);

                    wxThreadEvent* event = new wxThreadEvent(wxEVT_COMMAND_MYTHREAD_NOTIFICATION);
                    event->SetPayload(payload);
                    wxQueueEvent(m_pParent, event);
                }
                catch (json::parse_error& e) {
                    wxLogError("JSON parsing error: %s", e.what());
                }
            }
        }

        // Disconnect the pipe to allow another client to connect.
        DisconnectNamedPipe(hPipe);
    }

    CloseHandle(hPipe);
    return (wxThread::ExitCode)0;
}