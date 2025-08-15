#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <wx/wx.h>
#include <vector>

class Notification
{
public:
    wxString title;
    wxString channel;
    wxString sender;
    wxString time;
    wxString message;
    bool shown;
};

class Notifications
{
public:
    std::vector<Notification> notifications;
    void Add(Notification notification);
};

#endif // NOTIFICATIONS_H