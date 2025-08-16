#include "task_bar_icon.h"
#include "MyApp.h"

enum
{
    ID_TRAY_SHOW_NOTIFICATIONS = wxID_HIGHEST + 1,
    ID_TRAY_OPEN_MAIN_APP,
    ID_TRAY_EXIT
};

wxBEGIN_EVENT_TABLE(TaskBarIcon, wxTaskBarIcon)
    EVT_MENU(ID_TRAY_SHOW_NOTIFICATIONS, TaskBarIcon::OnMenuShowNotifications)
    EVT_MENU(ID_TRAY_OPEN_MAIN_APP, TaskBarIcon::OnMenuOpenMainApp)
    EVT_MENU(ID_TRAY_EXIT, TaskBarIcon::OnMenuExit)
wxEND_EVENT_TABLE()

TaskBarIcon::TaskBarIcon(MyApp* app) : m_app(app)
{
    wxIcon icon(wxT("images/logo64.ico"), wxBITMAP_TYPE_ICO);
    if (!icon.IsOk())
    {
        wxLogError(wxT("Could not load icon 'images/logo64.ico'."));
        return;
    }
    SetIcon(icon, wxT("Alerter"));
}

wxMenu* TaskBarIcon::CreatePopupMenu()
{
    wxMenu *menu = new wxMenu;
    menu->Append(ID_TRAY_SHOW_NOTIFICATIONS, wxT("Show Notifications"));
    menu->Append(ID_TRAY_OPEN_MAIN_APP, wxT("Open Main App"));
    menu->AppendSeparator();
    menu->Append(ID_TRAY_EXIT, wxT("Exit"));
    return menu;
}

void TaskBarIcon::OnMenuShowNotifications(wxCommandEvent&)
{
    m_app->StartNotifications();
}

void TaskBarIcon::OnMenuOpenMainApp(wxCommandEvent&)
{
    m_app->ShowWindow();
}

void TaskBarIcon::OnMenuExit(wxCommandEvent&)
{
    m_app->Exit();
}