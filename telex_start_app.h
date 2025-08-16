#pragma once

#include <wx/wx.h>
#include "notification_window.h"
#include "notification_timer.h"

class TaskBarIcon; // Forward declaration

class TelexStartApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
    void StartNotifications();
    void ShowWindow();

private:
    NotificationWindow* m_frame;
    NotificationTimer* m_timer;
    TaskBarIcon* m_taskBarIcon;
};
