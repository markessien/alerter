#include <wx/wx.h>
#include <wx/display.h>
#include "window.h"

class NotificationTimer : public wxTimer
{
public:
    NotificationTimer(NotificationWindow* frame);
    void Notify();
    void Start();

private:
    NotificationWindow* m_frame;
    int m_count;
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler);

    NotificationWindow* frame = new NotificationWindow(NULL, "TELEX");
    frame->Show(true);

    NotificationTimer* timer = new NotificationTimer(frame);
    timer->Start();

    return true;
}

NotificationTimer::NotificationTimer(NotificationWindow* frame)
    : m_frame(frame), m_count(0)
{
}

void NotificationTimer::Start()
{
    wxTimer::Start(2000); // 2-second interval
}

void NotificationTimer::Notify()
{
    switch (m_count)
    {
    case 0:
        m_frame->AddNotification("#mychannel", "Alertbot", "07:12", "It works, funnily enough!");
        break;
    case 1:
        m_frame->AddNotification("#anotherchannel", "User2", "08:30", "This is a second notification.");
        break;
    case 2:
        m_frame->AddNotification("#general", "Admin", "09:00", "This is the third and final notification.");
        Stop();
        break;
    }
    m_count++;
}
