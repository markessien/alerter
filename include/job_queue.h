#ifndef JOB_QUEUE_H
#define JOB_QUEUE_H

#include <wx/wx.h>
#include <wx/event.h>
#include <wx/thread.h>
#include <wx/msgqueue.h>
#include <map>

enum JobCommands
{
    CMD_EXIT,
    CMD_LOGIN,
};

struct Job
{
    JobCommands command;
    wxString string_arg1;
    wxString string_arg2;
};

class JobQueue
{
public:
    JobQueue(wxEvtHandler* pParent);
    void AddJob(const Job& job);
    Job Pop();
    void Report(const wxCommandEvent& event);

private:
    wxEvtHandler* m_pParent;
    wxMessageQueue<Job> m_queue;
    wxSemaphore m_semaphore;
    wxMutex m_mutex;
};

#endif // JOB_QUEUE_H