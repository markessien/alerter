#ifndef NOTIFICATION_WINDOW_H
#define NOTIFICATION_WINDOW_H

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/mstream.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <wx/sound.h>
#include <wx/timer.h>
#include "notifications.h"
#include "notification_content.h"
#include "telex.h" // Include Telex header

wxDECLARE_EVENT(wxEVT_COMMAND_APP_REQUESTEXIT, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_COMMAND_SHOW_LOGIN_DIALOG, wxCommandEvent); // New custom event
wxDECLARE_EVENT(wxEVT_COMMAND_TEST_NOTIFICATION, wxCommandEvent);

class TaskBarIcon;

class NotificationWindow : public wxFrame
{
public:
    NotificationWindow(wxWindow* parent, const wxString& title, Telex* telex); // Add Telex* parameter
    void AddNotification(const wxString& channel, const wxString& sender, const wxString& time, const wxString& message, const wxString& iconPath);
    void ShowLoginDialog(wxCommandEvent& event); // New function to show login dialog

private:
    void CreateNotificationWindow(wxWindow* parent, const wxString& title, int width, int headerHeight);
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
    void OnCloseButtonClick(wxCommandEvent& event);
    void OnPlaybackTimer(wxTimerEvent& event);
    void OnNotificationTimer(wxTimerEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnNotification(wxThreadEvent& event);
    void OnRequestExitApp(wxCommandEvent& event);
    void OnTestNotification(wxCommandEvent& event);
    void OnLoginSuccess(wxCommandEvent& event);
    void OnLoginFailure(wxCommandEvent& event);
    void OnWebSocketError(wxCommandEvent& event);
    void OnShowNotification(wxCommandEvent& event);

    wxPoint m_delta;
    NotificationManager notifications;
    wxPanel* backgroundPanel;
    wxBoxSizer* mainSizer;
    std::vector<NotificationContent*> m_contentAreas;
    wxTimer* m_playbackTimer;
    wxTimer* m_notificationTimer;
    TaskBarIcon* m_taskBarIcon;
    Telex* m_telex; // Pointer to Telex instance

    enum
    {
        ID_PlaybackTimer = wxID_HIGHEST + 1,
        ID_NotificationTimer
    };
};

#endif // NOTIFICATION_WINDOW_H