#include "MyApp.h"
#include "task_bar_icon.h"
#include "notification_timer.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxICOHandler);

    m_frame = new NotificationWindow(NULL, "TELEX");
    m_timer = new NotificationTimer(m_frame);
    m_taskBarIcon = new TaskBarIcon(this);

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

