#include "job_queue.h"

JobQueue::JobQueue(wxEvtHandler* pParent) : m_pParent(pParent)
{
}

void JobQueue::AddJob(const Job& job)
{
    wxMutexLocker lock(m_mutex);
    m_queue.Post(job);
    m_semaphore.Post();
}

Job JobQueue::Pop()
{
    m_semaphore.Wait();
    Job job;
    m_queue.Receive(job);
    return job;
}

void JobQueue::Report(const wxCommandEvent& event)
{
    wxQueueEvent(m_pParent, event.Clone());
}