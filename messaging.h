#pragma once
#include <wx/thread.h>
#include <wx/event.h>

wxDECLARE_EVENT(wxEVT_COMMAND_MYTHREAD_NOTIFICATION, wxThreadEvent);

class Messaging : public wxThread {
public:
    Messaging(wxEvtHandler* pParent);

protected:
    wxEvtHandler* m_pParent;
    virtual ExitCode Entry();
};