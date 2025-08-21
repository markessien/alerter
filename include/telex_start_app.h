#pragma once

#include <wx/wx.h>
#include "notification_window.h"
#include <wx/snglinst.h>
#include "telex.h" // Include Telex header

class Messaging; // Forward declaration

class TelexStartApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
    void ShowWindow();
    void OnActivateAnotherInstance();
    NotificationWindow* GetNotificationWindow() { return m_frame; }
    Telex* GetTelex() { return m_telex; } // Add getter for Telex instance

private:
    NotificationWindow* m_frame;
    Messaging* m_messaging;
    Telex* m_telex; // Pointer to Telex instance
    wxSingleInstanceChecker* m_checker;
};
