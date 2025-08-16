#pragma once

#include <wx/wx.h>
#include <wx/taskbar.h>

class TelexStartApp;

class TaskBarIcon : public wxTaskBarIcon
{
public:
    TaskBarIcon(TelexStartApp* app);
    virtual wxMenu *CreatePopupMenu();
#ifdef __WXMSW__
    virtual bool PopupMenu(wxMenu *menu);
#endif

private:
    void OnMenuShowNotifications(wxCommandEvent&);
    void OnMenuOpenMainApp(wxCommandEvent&);
    void OnMenuCloseMainApp(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);
    TelexStartApp* m_app;
#ifdef __WXMSW__
    PROCESS_INFORMATION m_processInformation;
#endif
    wxDECLARE_EVENT_TABLE();
};