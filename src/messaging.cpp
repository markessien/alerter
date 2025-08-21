#include "messaging.h"
#include "logger.h"
#include <thread>

wxDEFINE_EVENT(wxEVT_COMMAND_MYTHREAD_NOTIFICATION, wxThreadEvent);

Messaging::Messaging(wxEvtHandler* pParent)
    : m_pParent(pParent), m_httpThread() {}

Messaging::~Messaging() {
    Stop();
}

#include "http_server.h"

void Messaging::Start() {

    m_httpThread = std::thread(run_http_server, m_pParent);

}

void Messaging::Stop() {
    // Stop the HTTP server first
    
    stop_http_server();

    // Wait for the HTTP server thread to finish
    if (m_httpThread.joinable()) {
        m_httpThread.join();
    }


}