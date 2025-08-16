#include "telex_start_app.h"
#include "task_bar_icon.h"
#include "notification_timer.h"
#include "messaging.h"

bool TelexStartApp::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxICOHandler);

    m_frame = new NotificationWindow(NULL, "TELEX");
    m_timer = new NotificationTimer(m_frame);
    m_taskBarIcon = new TaskBarIcon(this);
    m_messaging = new Messaging(m_frame);

    if (m_messaging->Create() != wxTHREAD_NO_ERROR) {
        wxLogError("Could not create the messaging thread!");
        delete m_messaging;
        m_messaging = NULL;
        return false;
    }

    if (m_messaging->Run() != wxTHREAD_NO_ERROR) {
        wxLogError("Could not run the messaging thread!");
        delete m_messaging;
        m_messaging = NULL;
        return false;
    }

    return true;
}

int TelexStartApp::OnExit()
{
    delete m_taskBarIcon;
    if (m_messaging)
    {
        m_messaging->Delete();
        m_messaging = NULL;
    }
    return wxApp::OnExit();
}

void TelexStartApp::ShowWindow()
{
    if (m_frame)
    {
        m_frame->Show();
    }
}

