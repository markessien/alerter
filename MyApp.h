#pragma once

#include <wx/wx.h>
#include "window.h"

class MyTaskBarIcon; // Forward declaration
class NotificationTimer;

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
    void StartNotifications();
    void ShowWindow();

private:
    NotificationWindow* m_frame;
    NotificationTimer* m_timer;
    MyTaskBarIcon* m_taskBarIcon;
};

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