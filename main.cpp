#include <wx/wx.h>
#include <wx/display.h>
#include "window.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp); // Generates the main() function

bool MyApp::OnInit()
{
 // Required for loading PNGs from memory
    wxImage::AddHandler(new wxPNGHandler);

    NotificationWindow* frame = new NotificationWindow(NULL,
                                                       "TELEX - #mychannel",
                                                       "#mychannel",
                                                       "Alertbot",
                                                       "07:12",
                                                       "It works, funnily enough!");
    frame->Show(true);
    return true;
}

