#include "task_bar_icon.h"
#include "telex_start_app.h"
#include "notification_window.h"
#include <wx/app.h>
#include <wx/filefn.h>
#include "images.h"
#ifdef __WXMSW__
#include <windows.h>
#include <shellapi.h>
#endif

enum
{
    ID_TRAY_SHOW_NOTIFICATIONS = wxID_HIGHEST + 1,
    ID_TRAY_OPEN_MAIN_APP,
    ID_TRAY_CLOSE_MAIN_APP,
    ID_TRAY_TEST_NOTIFICATION,
    ID_TRAY_EXIT
};

wxBEGIN_EVENT_TABLE(TaskBarIcon, wxTaskBarIcon)
    EVT_MENU(ID_TRAY_OPEN_MAIN_APP, TaskBarIcon::OnMenuOpenMainApp)
    EVT_MENU(ID_TRAY_CLOSE_MAIN_APP, TaskBarIcon::OnMenuCloseMainApp)
    EVT_MENU(ID_TRAY_TEST_NOTIFICATION, TaskBarIcon::OnMenuTestNotification)
    EVT_MENU(ID_TRAY_EXIT, TaskBarIcon::OnMenuExit)
wxEND_EVENT_TABLE()

TaskBarIcon::TaskBarIcon(TelexStartApp* app) : m_app(app)
{
#ifdef __WXMSW__
    ZeroMemory(&m_processInformation, sizeof(m_processInformation));
#endif

    wxMemoryInputStream iconStream(_aclogo64, _aclogo64_size);

    wxImage img;
    if (!img.LoadFile(iconStream, wxBITMAP_TYPE_ICO)) // or ICO if that's your data
    {
        wxLogError("Failed to load icon data.");
        return;
    }

    wxIcon icon;
    icon.CopyFromBitmap(wxBitmap(img));


    //icon.LoadFile("IDI_APP_ICON", wxBITMAP_TYPE_ICO_RESOURCE);
    /*
    if (!icon.LoadFile(iconStream, wxBITMAP_TYPE_ICO))
    {
        wxLogError(wxT("Could not load icon IDI_APP_ICON."));
        return;
    }
    */
    SetIcon(icon, wxT("Alerter"));
}

wxMenu* TaskBarIcon::CreatePopupMenu()
{
    wxMenu *menu = new wxMenu;
    menu->Append(ID_TRAY_OPEN_MAIN_APP, wxT("Open Main App"));
    menu->Append(ID_TRAY_CLOSE_MAIN_APP, wxT("Close Main App"));
    menu->AppendSeparator();
    menu->Append(ID_TRAY_TEST_NOTIFICATION, wxT("Test Notifications"));
    menu->AppendSeparator();
    menu->Append(ID_TRAY_EXIT, wxT("Exit"));
    return menu;
}

void TaskBarIcon::OnMenuOpenMainApp(wxCommandEvent&)
{
#ifdef __WXMSW__
    if (m_processInformation.hProcess != NULL)
    {
        DWORD exitCode;
        GetExitCodeProcess(m_processInformation.hProcess, &exitCode);
        if (exitCode == STILL_ACTIVE)
        {
            wxLogMessage("Main app is already running.");
            return;
        }
    }

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&m_processInformation, sizeof(m_processInformation));

    wxString command = ".\\testing\\flutter-test-app\\alerter_test_app\\build\\windows\\x64\\runner\\Debug\\alerter_test_app.exe";
    if (!wxFileExists(command))
    {
        command = "telex.exe";
    }

    if (!CreateProcess(NULL, command.wchar_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &m_processInformation))
    {
        wxLogError("Could not start main app.");
    }
#endif
}

void TaskBarIcon::OnMenuCloseMainApp(wxCommandEvent&)
{
#ifdef __WXMSW__
    if (m_processInformation.hProcess != NULL)
    {
        DWORD exitCode;
        GetExitCodeProcess(m_processInformation.hProcess, &exitCode);
        if (exitCode != STILL_ACTIVE)
        {
            ZeroMemory(&m_processInformation, sizeof(m_processInformation));
            wxLogMessage("Main app is not running.");
            return;
        }

        if (TerminateProcess(m_processInformation.hProcess, 0))
        {
            CloseHandle(m_processInformation.hProcess);
            CloseHandle(m_processInformation.hThread);
            ZeroMemory(&m_processInformation, sizeof(m_processInformation));
            wxLogMessage("Main app closed.");
        }
        else
        {
            wxLogError("Could not close main app.");
        }
    }
#endif
}

void TaskBarIcon::OnMenuExit(wxCommandEvent&)
{
    m_app->Exit();
}

void TaskBarIcon::OnMenuTestNotification(wxCommandEvent&)
{
    NotificationWindow* notificationWindow = m_app->GetNotificationWindow();
    if (notificationWindow)
    {
        notificationWindow->AddNotification("Test Channel", "Test Sender", "Now", "This is a test notification.", "");
    }
}
#ifdef __WXMSW__
bool TaskBarIcon::PopupMenu(wxMenu *menu)
{
    return wxTaskBarIcon::PopupMenu(menu);
}
#endif