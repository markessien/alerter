#ifndef NOTIFICATION_CONTENT_AREA_H
#define NOTIFICATION_CONTENT_AREA_H

#include <wx/wx.h>

class NotificationContentArea : public wxPanel
{
public:
    NotificationContentArea(wxWindow* parent,
        const wxString& channel,
        const wxString& sender,
        const wxString& time,
        const wxString& message,
        int width,
        int top,
        int left);
};

#endif // NOTIFICATION_CONTENT_AREA_H