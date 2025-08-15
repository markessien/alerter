#include "window.h"
#include "icons.h"
#ifdef __WXMSW__
#include <windows.h>
#endif

void NotificationWindow::CreateNotificationWindow(wxWindow* parent,
    const wxString& title,
    int width,
    int headerHeight)
{
    wxBitmap closeBitmap = wxBitmapBundle::FromSVGFile(wxT("images/x.svg"), wxSize(16, 16)).GetBitmap(wxDefaultSize);
    wxImage titleBarImage(wxT("images/small_logo28.png"), wxBITMAP_TYPE_PNG);
    wxBitmap titleBarBitmap(titleBarImage);

    if (!closeBitmap.IsOk() || !titleBarBitmap.IsOk())
    {
        wxMessageBox(
            "Could not load image resources.\n"
            "Please make sure the icons exist in the folder.",
            "Error", wxOK | wxICON_ERROR);
        wxPendingDelete.Append(this);
        return;
    }

    backgroundPanel = new wxPanel(this);
    backgroundPanel->SetBackgroundColour(*wxWHITE);
    mainSizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* titleBarPanel = new wxPanel(backgroundPanel);
    titleBarPanel->SetBackgroundColour(wxColour(74, 55, 168));
    wxBoxSizer* titleBarSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBitmap* titleBarIcon = new wxStaticBitmap(titleBarPanel, wxID_ANY, titleBarBitmap);

    wxStaticText* titleText = new wxStaticText(titleBarPanel, wxID_ANY, title);
    titleText->SetForegroundColour(*wxWHITE);
    titleText->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Segoe UI"));

    wxBitmapButton* closeButton = new wxBitmapButton(titleBarPanel, wxID_ANY, closeBitmap, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    closeButton->SetBackgroundColour(wxColour(74, 55, 168));

    titleBarSizer->Add(titleBarIcon, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    titleBarSizer->Add(titleText, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
    titleBarSizer->Add(closeButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 16);
    titleBarPanel->SetSizer(titleBarSizer);

    mainSizer->Add(titleBarPanel, 0, wxEXPAND | wxFIXED_MINSIZE);
    titleBarPanel->SetMinSize(wxSize(-1, headerHeight));

    backgroundPanel->SetSizer(mainSizer);

    wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(backgroundPanel, 1, wxEXPAND);

    this->SetSizerAndFit(frameSizer);
    this->SetClientSize(wxSize(width, headerHeight));

    titleBarPanel->Bind(wxEVT_LEFT_DOWN, &NotificationWindow::OnMouseDown, this);
    titleBarPanel->Bind(wxEVT_LEFT_UP, &NotificationWindow::OnMouseUp, this);
    titleBarPanel->Bind(wxEVT_MOTION, &NotificationWindow::OnMouseMove, this);
    titleText->Bind(wxEVT_LEFT_DOWN, &NotificationWindow::OnMouseDown, this);
    titleText->Bind(wxEVT_LEFT_UP, &NotificationWindow::OnMouseUp, this);
    titleText->Bind(wxEVT_MOTION, &NotificationWindow::OnMouseMove, this);
    titleBarIcon->Bind(wxEVT_LEFT_DOWN, &NotificationWindow::OnMouseDown, this);
    titleBarIcon->Bind(wxEVT_LEFT_UP, &NotificationWindow::OnMouseUp, this);
    titleBarIcon->Bind(wxEVT_MOTION, &NotificationWindow::OnMouseMove, this);
    closeButton->Bind(wxEVT_BUTTON, &NotificationWindow::OnCloseButtonClick, this);
    this->Bind(wxEVT_MOUSE_CAPTURE_LOST, &NotificationWindow::OnMouseCaptureLost, this);

    wxRect screenRect = wxGetClientDisplayRect();
    wxSize windowSize = GetSize();
    SetPosition(wxPoint(screenRect.width - windowSize.x - 12, screenRect.height - windowSize.y - 12));
}

NotificationWindow::NotificationWindow(wxWindow* parent,
    const wxString& title)
    : wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxSTAY_ON_TOP)
{
    CreateNotificationWindow(parent, title, 350, 40);
#ifdef __WXMSW__
    HWND hwnd = (HWND)this->GetHandle();
    LONG_PTR style = GetClassLongPtr(hwnd, GCL_STYLE);
    SetClassLongPtr(hwnd, GCL_STYLE, style | CS_DROPSHADOW);
#endif
}

void NotificationWindow::AddNotification(const wxString& channel, const wxString& sender, const wxString& time, const wxString& message)
{
    Notification notification;
    notification.channel = channel;
    notification.sender = sender;
    notification.time = time;
    notification.message = message;
    notification.shown = false;
    notifications.Add(notification);

    wxPoint oldPos = GetPosition();
    wxSize oldSize = GetSize();

    NotificationContentArea* contentArea = new NotificationContentArea(backgroundPanel, channel, sender, time, message, 350, 0, 0);
    mainSizer->Add(contentArea, 1, wxEXPAND);
    
    this->Layout();
    this->Fit();

    wxSize newSize = GetSize();
    int heightIncrease = newSize.GetHeight() - oldSize.GetHeight();

    if (heightIncrease > 0)
    {
        SetPosition(wxPoint(oldPos.x, oldPos.y - heightIncrease));
    }
}

void NotificationWindow::OnMouseDown(wxMouseEvent& event)
{
    CaptureMouse();
    wxPoint pos = ClientToScreen(event.GetPosition());
    wxPoint origin = GetPosition();
    m_delta = pos - origin;
}

void NotificationWindow::OnMouseMove(wxMouseEvent& event)
{
    const wxMouseState mouseState = wxGetMouseState();
    if (mouseState.LeftIsDown())
    {
        wxPoint pos = ClientToScreen(event.GetPosition());
        SetPosition(pos - m_delta);
    }
}

void NotificationWindow::OnMouseUp(wxMouseEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
}

void NotificationWindow::OnCloseButtonClick(wxCommandEvent& event)
{
    Close(true);
}

void NotificationWindow::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
}