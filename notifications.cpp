#include "notifications.h"

#include <algorithm>

void Notifications::Add(Notification notification)
{
    notification.creationTime = wxDateTime::Now();
    notifications.push_back(notification);
}

void Notifications::RemoveOld()
{
    if (!notifications.empty())
    {
        const auto& oldest = notifications.front();
        if (wxDateTime::Now() - oldest.creationTime > wxTimeSpan::Seconds(7))
        {
            notifications.erase(notifications.begin());
        }
    }
}