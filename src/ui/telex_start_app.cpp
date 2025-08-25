#include "telex_start_app.h"
#include "task_bar_icon.h"
#include "messaging.h"
#include "telex.h" // Include Telex header

bool TelexStartApp::OnInit()
{
    SetVendorName("Telex.im");     // or your Verein/project name
    SetAppName("Telex");       // your app name

    m_checker = new wxSingleInstanceChecker;
    if (m_checker->IsAnotherRunning())
    {
        delete m_checker;
        m_checker = NULL;
        return false;
    }

    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxICOHandler);

    m_frame = new NotificationWindow(NULL, "TELEX");
    m_telex = new Telex(m_frame);
    m_frame->SetTelex(m_telex);
    m_telex->Run();
    m_messaging = new Messaging(m_frame);

    m_messaging->Start();

    return true;
}

int TelexStartApp::OnExit()
{
    if (m_messaging)
    {
        m_messaging->Stop();
        delete m_messaging;
        m_messaging = NULL;
    }
    if (m_telex)
    {
        m_telex->Exit();
        m_telex->Wait();
        delete m_telex;
        m_telex = NULL;
    }
    if (m_checker)
    {
        delete m_checker;
        m_checker = NULL;
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

void TelexStartApp::OnActivateAnotherInstance()
{
}

