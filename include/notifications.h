#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <wx/wx.h>
#include <wx/datetime.h>
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
    wxDateTime creationTime;
};

class NotificationManager
{
public:
    std::vector<Notification> notifications;
    void Add(Notification notification);
    void RemoveOld();
};

#endif // NOTIFICATIONS_H