#include "window.h"
#include "icons.h"



void NotificationWindow::CreateNotificationWindow(wxWindow* parent,
    const wxString& title,
    const wxString& channel,
    const wxString& sender,
    const wxString& time,
    const wxString& message,
    int width,
    int height,
    int headerHeight)
{
    // Todo, this is inefficient, we need to pre size these imeges
    // --- Load Bitmaps from Files ---
    wxBitmap closeBitmap = wxBitmapBundle::FromSVGFile(wxT("images/x.svg"), wxSize(16, 16)).GetBitmap(wxDefaultSize);
    wxImage alertUserImage(wxT("images/logo48.png"), wxBITMAP_TYPE_PNG);
    // userImage.Rescale(64, 64);
    wxBitmap alertUserBitmap(alertUserImage);

    wxImage titleBarImage(wxT("images/small_logo28.png"), wxBITMAP_TYPE_PNG);
    // logoVerySmallImage.Rescale(28, 28);
    wxBitmap titleBarBitmap(titleBarImage);

    // --- Error Checking ---
    if (!closeBitmap.IsOk() || !alertUserBitmap.IsOk() || !titleBarBitmap.IsOk())
    {
        wxMessageBox(
            "Could not load image resources.\n"
            "Please make sure the icons exist in the folder.",
            "Error", wxOK | wxICON_ERROR);
        // Destroy the frame cleanly and exit if images are missing
        wxPendingDelete.Append(this);
        return;
    }

    // --- Create Panels and Sizers ---
    // This is now the single main panel for the frame
    wxPanel* backgroundPanel = new wxPanel(this);
    backgroundPanel->SetBackgroundColour(*wxWHITE);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* titleBarPanel = new wxPanel(backgroundPanel);
    titleBarPanel->SetBackgroundColour(wxColour(74, 55, 168));
    wxBoxSizer* titleBarSizer = new wxBoxSizer(wxHORIZONTAL);

    wxPanel* contentPanel = new wxPanel(backgroundPanel);
    contentPanel->SetBackgroundColour(*wxWHITE);
    wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);

    // --- Populate Title Bar ---
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

    // --- Populate Content Area ---
    wxBoxSizer* contentBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBitmap* alertUserIcon = new wxStaticBitmap(contentPanel, wxID_ANY, alertUserBitmap);

    wxBoxSizer* textSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* senderSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* senderText = new wxStaticText(contentPanel, wxID_ANY, sender);
    senderText->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Segoe UI"));
    wxStaticText* timeText = new wxStaticText(contentPanel, wxID_ANY, time);
    timeText->SetForegroundColour(wxColour(160, 160, 160));
    timeText->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Segoe UI"));
    senderSizer->Add(senderText, 0, wxALIGN_CENTER_VERTICAL);
    senderSizer->AddStretchSpacer(1);
    senderSizer->Add(timeText, 0, wxALIGN_CENTER_VERTICAL);

    wxStaticText* messageText = new wxStaticText(contentPanel, wxID_ANY, message);
    messageText->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Segoe UI"));
    messageText->Wrap(width - 80); // Adjust wrap width

    textSizer->Add(senderSizer, 0, wxEXPAND);
    textSizer->Add(messageText, 1, wxEXPAND | wxTOP, 5);

    contentBoxSizer->Add(alertUserIcon, 0, wxALIGN_TOP | wxALL, 10);
    contentBoxSizer->Add(textSizer, 1, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 10);

    contentPanel->SetSizer(contentBoxSizer);

    // --- Final Layout (Corrected) ---
    mainSizer->Add(titleBarPanel, 0, wxEXPAND | wxFIXED_MINSIZE);
    titleBarPanel->SetMinSize(wxSize(-1, headerHeight));
    mainSizer->Add(contentPanel, 1, wxEXPAND);

    // Set the sizer for the main panel that contains all the controls
    backgroundPanel->SetSizer(mainSizer);

    // Create a new sizer for the frame itself
    wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
    // Add the main panel to the frame's sizer, making it expand to fill the frame
    frameSizer->Add(backgroundPanel, 1, wxEXPAND);

    // Set the sizer for the frame and fit the frame to the sizer's content
    this->SetSizerAndFit(frameSizer);
    this->SetClientSize(wxSize(width, height));


    // --- Bind Events ---
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

    wxRect screenRect = wxGetClientDisplayRect();
    wxSize windowSize = GetSize();
    SetPosition(wxPoint(screenRect.width - windowSize.x - 20, screenRect.height - windowSize.y - 40));
}

NotificationWindow::NotificationWindow(wxWindow* parent,
    const wxString& title,
    const wxString& channel,
    const wxString& sender,
    const wxString& time,
    const wxString& message)
    : wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxSTAY_ON_TOP)
{
    CreateNotificationWindow(parent, title, channel, sender, time, message, 350, 120, 40);
}


// --- Event Handler Implementations ---

void NotificationWindow::OnMouseDown(wxMouseEvent& event)
{
    CaptureMouse();
    wxPoint pos = ClientToScreen(event.GetPosition());
    wxPoint origin = GetPosition();
    m_delta = pos - origin;
}

void NotificationWindow::OnMouseMove(wxMouseEvent& event)
{
    if (event.Dragging() && event.LeftIsDown())
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