#pragma once

#include <wx/wx.h>
#include "notification_window.h"
#include <wx/snglinst.h>

class TaskBarIcon; // Forward declaration
class Messaging; // Forward declaration

class TelexStartApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
    void ShowWindow();
    void OnActivateAnotherInstance();

private:
    NotificationWindow* m_frame;
    TaskBarIcon* m_taskBarIcon;
    Messaging* m_messaging;
    wxSingleInstanceChecker* m_checker;
};
