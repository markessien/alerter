#ifndef WINDOW_H
#define WINDOW_H

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/mstream.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include "notifications.h"
#include "notification_content_area.h"

class NotificationWindow : public wxFrame
{
public:
    NotificationWindow(wxWindow* parent, const wxString& title);
    void AddNotification(const wxString& channel, const wxString& sender, const wxString& time, const wxString& message);

private:
    void CreateNotificationWindow(wxWindow* parent, const wxString& title, int width, int headerHeight);
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
    void OnCloseButtonClick(wxCommandEvent& event);

    wxPoint m_delta;
    Notifications notifications;
    wxPanel* backgroundPanel;
    wxBoxSizer* mainSizer;
};

#endif // WINDOW_H