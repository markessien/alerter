#include <winsock2.h>
#include "writer_thread.h"
#include "logger.h"
#include "telex_protocol.h"
#include <json.hpp>
#include "pipe.h"

using json = nlohmann::json;

WriterThread::WriterThread(wxMessageQueue<std::string>& queue)
    : wxThread(wxTHREAD_JOINABLE), m_responseQueue(queue) {}

wxThread::ExitCode WriterThread::Entry() {

    /*
    Pipe s2cPipe(L"\\\\.\\pipe\\telex-notifications-s2c", PIPE_ACCESS_OUTBOUND);
    if (!s2cPipe.IsValid()) {
        return (wxThread::ExitCode)1;
    }

    while (!TestDestroy()) {
        if (s2cPipe.Connect()) {
            while (!TestDestroy()) {
                std::string response;
                if (m_responseQueue.Receive(response) == wxMSGQUEUE_NO_ERROR) {
                    if (response == "shutdown") break;

                    size_t separator_pos = response.find(':');
                    std::string status = response.substr(0, separator_pos);
                    std::string details = response.substr(separator_pos + 1);
                    
                    json response_json;
                    response_json["status"] = status;
                    response_json["details"] = details;

                    std::string message_to_send = TelexProtocol::FrameMessage(response_json);
                    DWORD dwWritten;
                    s2cPipe.Write(message_to_send.c_str(), message_to_send.length(), dwWritten);
                } else {
                    break; // Queue was deleted
                }
            }
            s2cPipe.Disconnect();
        }
    }

    */
    return (wxThread::ExitCode)0;
}