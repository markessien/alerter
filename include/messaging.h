#pragma once
#include <wx/wx.h>
#include <wx/msgqueue.h>
#include <string>
#include "http_server.h"
#include <thread>

wxDECLARE_EVENT(wxEVT_COMMAND_MYTHREAD_NOTIFICATION, wxThreadEvent);

class Messaging {
public:
    Messaging(wxEvtHandler* pParent);
    ~Messaging();

    void Start();
    void Stop();

private:
    wxEvtHandler* m_pParent;
    std::thread m_httpThread;
};