#pragma once

#include <wx/wx.h>
#include <wx/taskbar.h>

class MyApp;

class MyTaskBarIcon : public wxTaskBarIcon
{
public:
    MyTaskBarIcon(MyApp* app);
    virtual wxMenu *CreatePopupMenu();

private:
    void OnMenuShowNotifications(wxCommandEvent&);
    void OnMenuOpenMainApp(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);
    MyApp* m_app;
    wxDECLARE_EVENT_TABLE();
};