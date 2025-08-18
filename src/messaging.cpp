#include "messaging.h"
#include "logger.h"
#include <thread>

wxDEFINE_EVENT(wxEVT_COMMAND_MYTHREAD_NOTIFICATION, wxThreadEvent);

Messaging::Messaging(wxEvtHandler* pParent)
    : m_pParent(pParent), m_readerThread(nullptr), m_writerThread(nullptr), m_httpThread() {}

Messaging::~Messaging() {
    Stop();
}

#include "http_server.h"

void Messaging::Start() {
    m_readerThread = new ReaderThread(m_pParent, m_responseQueue);
    m_writerThread = new WriterThread(m_responseQueue);

    if (m_readerThread->Run() != wxTHREAD_NO_ERROR || m_writerThread->Run() != wxTHREAD_NO_ERROR) {
        LogMessage("Failed to start messaging threads.");
        delete m_readerThread;
        delete m_writerThread;
        m_readerThread = nullptr;
        m_writerThread = nullptr;
    }

    m_httpThread = std::thread(run_http_server, m_pParent);
    m_httpThread.detach();
}

void Messaging::Stop() {
    stop_http_server();

    // Signal the writer thread to shut down
    if (m_writerThread) {
        m_responseQueue.Post("shutdown");
    }

    // Wait for the reader thread to terminate
    if (m_readerThread) {
        if (m_readerThread->IsRunning()) {
            m_readerThread->Wait();
        }
        delete m_readerThread;
        m_readerThread = nullptr;
    }

    // Wait for the writer thread to terminate
    if (m_writerThread) {
        if (m_writerThread->IsRunning()) {
            m_writerThread->Wait();
        }
        delete m_writerThread;
        m_writerThread = nullptr;
    }
}