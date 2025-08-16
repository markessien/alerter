#include "MyApp.h"
#include "TrayIcon.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxICOHandler);

    m_frame = new NotificationWindow(NULL, "TELEX");
    m_timer = new NotificationTimer(m_frame);
    m_taskBarIcon = new MyTaskBarIcon(this);

    return true;
}

int MyApp::OnExit()
{
    delete m_taskBarIcon;
    return wxApp::OnExit();
}

void MyApp::StartNotifications()
{
    if (m_timer)
    {
        m_timer->Start();
    }
}

void MyApp::ShowWindow()
{
    if (m_frame)
    {
        m_frame->Show();
    }
}

NotificationTimer::NotificationTimer(NotificationWindow* frame)
    : m_frame(frame), m_count(0)
{
}

void NotificationTimer::Start()
{
    m_count = 0;
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
        m_frame->AddNotification("", "User2", "08:30", "This is a second notification.");
        break;
    case 2:
        m_frame->AddNotification("#general", "Admin", "09:00", "This is a much longer notification that should wrap to two lines and then be truncated with an ellipsis to show that there is more text than can be displayed.");
        break;
    case 3:
        m_frame->AddNotification("#final", "Bot", "09:01", "This is the final notification.");
        Stop();
        break;
    }
    m_count++;
}
