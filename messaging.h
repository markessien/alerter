#pragma once
#include <wx/thread.h>

class Messaging : public wxThread {
public:
    Messaging();

protected:
    virtual ExitCode Entry();
};