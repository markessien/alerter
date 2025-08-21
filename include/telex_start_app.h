#pragma once

#include <wx/wx.h>
#include "notification_window.h"
#include <wx/snglinst.h>

class Messaging; // Forward declaration

class TelexStartApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
    void ShowWindow();
    void OnActivateAnotherInstance();
    NotificationWindow* GetNotificationWindow() { return m_frame; }

private:
    NotificationWindow* m_frame;
    Messaging* m_messaging;
    wxSingleInstanceChecker* m_checker;
};
