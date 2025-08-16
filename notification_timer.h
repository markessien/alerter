#pragma once

#include <wx/wx.h>
#include "notification_window.h"

class NotificationTimer : public wxTimer
{
public:
    NotificationTimer(NotificationWindow* frame);
    void Notify();
    void Start();

private:
    NotificationWindow* m_frame;
    int m_count;
};