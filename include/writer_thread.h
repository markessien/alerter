#pragma once
#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/msgqueue.h>
#include <string>

class WriterThread : public wxThread {
public:
    WriterThread(wxMessageQueue<std::string>& queue);

protected:
    virtual ExitCode Entry();

private:
    wxMessageQueue<std::string>& m_responseQueue;
};