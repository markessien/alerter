#pragma once
#include <wx/wx.h>
#include <wx/msgqueue.h>
#include <string>
#include "reader_thread.h"
#include "writer_thread.h"

wxDECLARE_EVENT(wxEVT_COMMAND_MYTHREAD_NOTIFICATION, wxThreadEvent);

class Messaging {
public:
    Messaging(wxEvtHandler* pParent);
    ~Messaging();

    void Start();
    void Stop();

private:
    wxEvtHandler* m_pParent;
    ReaderThread* m_readerThread;
    WriterThread* m_writerThread;
    wxMessageQueue<std::string> m_responseQueue;
};