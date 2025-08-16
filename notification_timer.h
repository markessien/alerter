#pragma once

#include <wx/wx.h>
#include "notification_window.h"

class NotificationTimer : public wxTimer
{
public:
    NotificationTimer(NotificationWindow* frame);
private:
    NotificationWindow* m_frame;
    int m_count;
};