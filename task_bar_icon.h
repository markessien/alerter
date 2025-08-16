#pragma once

#include <wx/wx.h>
#include <wx/taskbar.h>

class MyApp;

class TaskBarIcon : public wxTaskBarIcon
{
public:
    TaskBarIcon(MyApp* app);
    virtual wxMenu *CreatePopupMenu();
#ifdef __WXMSW__
    virtual bool PopupMenu(wxMenu *menu);
#endif

private:
    void OnMenuShowNotifications(wxCommandEvent&);
    void OnMenuOpenMainApp(wxCommandEvent&);
    void OnMenuCloseMainApp(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);
    MyApp* m_app;
#ifdef __WXMSW__
    PROCESS_INFORMATION m_processInformation;
#endif
    wxDECLARE_EVENT_TABLE();
};