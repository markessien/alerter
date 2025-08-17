#pragma once
#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/msgqueue.h>
#include <string>

class ReaderThread : public wxThread {
public:
    ReaderThread(wxEvtHandler* parent, wxMessageQueue<std::string>& queue);

protected:
    virtual ExitCode Entry();

private:
    wxEvtHandler* m_pParent;
    wxMessageQueue<std::string>& m_responseQueue;
};