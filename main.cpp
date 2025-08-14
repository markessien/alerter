#include <wx/wx.h>
#include <wx/display.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);
};

wxIMPLEMENT_APP(MyApp); // Generates the main() function

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Hello World from VS Code");
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title)
{
    new wxStaticText(this, wxID_ANY, "It works!", wxPoint(50, 50));

    // Get the usable screen area (excludes the taskbar)
    wxRect screenRect = wxDisplay().GetClientArea();

    // Get the size of this frame
    wxSize frameSize = GetSize();

    // Calculate the new position for the bottom-right corner ➡️
    int x = screenRect.GetRight() - frameSize.GetWidth();
    int y = screenRect.GetBottom() - frameSize.GetHeight();

    // Set the new position
    SetPosition(wxPoint(x, y));
}