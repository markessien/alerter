#include "TrayIcon.h"
#include "MyApp.h"

enum
{
    ID_TRAY_SHOW_NOTIFICATIONS = wxID_HIGHEST + 1,
    ID_TRAY_OPEN_MAIN_APP,
    ID_TRAY_EXIT
};

wxBEGIN_EVENT_TABLE(MyTaskBarIcon, wxTaskBarIcon)
    EVT_MENU(ID_TRAY_SHOW_NOTIFICATIONS, MyTaskBarIcon::OnMenuShowNotifications)
    EVT_MENU(ID_TRAY_OPEN_MAIN_APP, MyTaskBarIcon::OnMenuOpenMainApp)
    EVT_MENU(ID_TRAY_EXIT, MyTaskBarIcon::OnMenuExit)
wxEND_EVENT_TABLE()

MyTaskBarIcon::MyTaskBarIcon(MyApp* app) : m_app(app)
{
    wxIcon icon(wxT("images/logo64.ico"), wxBITMAP_TYPE_ICO);
    if (!icon.IsOk())
    {
        wxLogError(wxT("Could not load icon 'images/logo64.ico'."));
        return;
    }
    SetIcon(icon, wxT("Alerter"));
}

wxMenu* MyTaskBarIcon::CreatePopupMenu()
{
    wxMenu *menu = new wxMenu;
    menu->Append(ID_TRAY_SHOW_NOTIFICATIONS, wxT("Show Notifications"));
    menu->Append(ID_TRAY_OPEN_MAIN_APP, wxT("Open Main App"));
    menu->AppendSeparator();
    menu->Append(ID_TRAY_EXIT, wxT("Exit"));
    return menu;
}

void MyTaskBarIcon::OnMenuShowNotifications(wxCommandEvent&)
{
    m_app->StartNotifications();
}

void MyTaskBarIcon::OnMenuOpenMainApp(wxCommandEvent&)
{
    m_app->ShowWindow();
}

void MyTaskBarIcon::OnMenuExit(wxCommandEvent&)
{
    m_app->Exit();
}