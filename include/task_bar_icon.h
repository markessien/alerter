#pragma once

#include <wx/wx.h>
#include <wx/taskbar.h>

wxDECLARE_EVENT(wxEVT_COMMAND_APP_REQUESTEXIT, wxCommandEvent);

class TaskBarIcon : public wxTaskBarIcon
{
public:
    TaskBarIcon(wxWindow* parent);
    virtual wxMenu *CreatePopupMenu();
#ifdef __WXMSW__
    virtual bool PopupMenu(wxMenu *menu);
#endif

private:
    void OnMenuShowNotifications(wxCommandEvent&);
    void OnMenuOpenMainApp(wxCommandEvent&);
    void OnMenuCloseMainApp(wxCommandEvent&);
    void OnMenuTestNotification(wxCommandEvent&);
    void OnMenuLogin(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);
    wxWindow* m_parent;
#ifdef __WXMSW__
    PROCESS_INFORMATION m_processInformation;
#endif
    wxDECLARE_EVENT_TABLE();
};