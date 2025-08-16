#ifndef NOTIFICATION_CONTENT_H
#define NOTIFICATION_CONTENT_H

#include <wx/wx.h>

class NotificationContent : public wxPanel
{
public:
    NotificationContent(wxWindow* parent,
        const wxString& channel,
        const wxString& sender,
        const wxString& time,
        const wxString& message,
        const wxString& iconPath,
        int width,
        int top,
        int left);
};

#endif // NOTIFICATION_CONTENT_H