#include <wx/wx.h>

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
}