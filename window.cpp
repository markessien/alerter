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

    wxStaticText* titleText = new wxStaticText(titleBarPanel, wxID_ANY, "TELEX");
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
    : wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxSTAY_ON_TOP | wxFRAME_NO_TASKBAR)
{
    CreateNotificationWindow(parent, title, 350, 40);
    m_playbackTimer = new wxTimer(this, ID_PlaybackTimer);
    m_notificationTimer = new wxTimer(this, ID_NotificationTimer);
    Bind(wxEVT_TIMER, &NotificationWindow::OnPlaybackTimer, this, ID_PlaybackTimer);
    Bind(wxEVT_TIMER, &NotificationWindow::OnNotificationTimer, this, ID_NotificationTimer);
    Bind(wxEVT_CLOSE_WINDOW, &NotificationWindow::OnClose, this);
    m_notificationTimer->Start(1000);
#ifdef __WXMSW__
    HWND hwnd = (HWND)this->GetHandle();
    LONG_PTR style = GetClassLongPtr(hwnd, GCL_STYLE);
    SetClassLongPtr(hwnd, GCL_STYLE, style | CS_DROPSHADOW);
#endif
}

void NotificationWindow::AddNotification(const wxString& channel, const wxString& sender, const wxString& time, const wxString& message)
{
    Freeze();

    if (!IsShown())
    {
        Show();
    }

    Notification notification;
    notification.channel = channel;
    notification.sender = sender;
    notification.time = time;
    notification.message = message;
    notification.shown = false;
    notifications.Add(notification);

    // 1. Build the new content area without modifying the existing window
    NotificationContentArea* contentArea = new NotificationContentArea(backgroundPanel, channel, sender, time, message, 350, 0, 0);
    contentArea->SetSize(wxSize(350, contentArea->GetBestSize().y));
    contentArea->Hide();

    // 2. Add the new one to the window, but offscreen
    wxSize contentSize = contentArea->GetSize();
    contentArea->SetPosition(wxPoint(0, GetClientSize().y));

    // 3. Resize the window first, then move existing notifications upwards
    wxPoint oldPos = GetPosition();
    wxSize oldSize = GetSize();
    wxSize newSize(oldSize.x, oldSize.y + contentSize.y);
    SetSize(newSize);
    SetPosition(wxPoint(oldPos.x, oldPos.y - contentSize.y));

    int top = 40;
    for (size_t i = 0; i < m_contentAreas.size(); ++i)
    {
        m_contentAreas[i]->Move(0, top);
        top += m_contentAreas[i]->GetSize().y;
    }

    // 4. Slot the newest one into the bottom
    contentArea->SetPosition(wxPoint(0, top));
    contentArea->Show();
    m_contentAreas.push_back(contentArea);

    Thaw();

    m_playbackTimer->StartOnce(1);
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

void NotificationWindow::OnPlaybackTimer(wxTimerEvent& event)
{
    wxSound::Play(wxT("audio/knock.wav"), wxSOUND_ASYNC);
}

void NotificationWindow::OnNotificationTimer(wxTimerEvent& event)
{
    if (m_contentAreas.empty())
    {
        return;
    }

    int old_notification_count = notifications.notifications.size();
    notifications.RemoveOld();
    if (notifications.notifications.size() == old_notification_count)
    {
        return;
    }

    Freeze();

    NotificationContentArea* topArea = m_contentAreas.front();
    int topHeight = topArea->GetSize().y;
    topArea->Destroy();
    m_contentAreas.erase(m_contentAreas.begin());

    int top = 40;
    for (auto& area : m_contentAreas)
    {
        area->Move(0, top);
        top += area->GetSize().y;
    }

    wxPoint oldPos = GetPosition();
    wxSize oldSize = GetSize();
    wxSize newSize(oldSize.x, oldSize.y - topHeight);
    SetSize(newSize);
    SetPosition(wxPoint(oldPos.x, oldPos.y + topHeight));

    Thaw();

    if (m_contentAreas.empty())
    {
        this->Hide();
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
    this->Hide();
}

void NotificationWindow::OnClose(wxCloseEvent& event)
{
    this->Hide();
}

void NotificationWindow::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
}
