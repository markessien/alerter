#include "window.h"
#include "icons.h"


// A simple 24x24 white 'X' icon for the close button
const char* close_icon_base64 =
    "iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAAABHNCSVQICAgIfAhkiAAAAAlwSFlz"
    "AAAAbwAAAG8B8aLcQwAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAB5SURB"
    "VEiJ7dYxCsAgEETR5t7/Y1sRBBWstA0N4uWwF8LCX8w548E/OPsCksAYoMEYsAFWYAnEwBqYgDmw"
    "A1ZgBfKZAQ+gA1qgA/YlC56ADFiBDlgBf+0y8B+YtQO2YAlgwA4YgS0wA0OwBUZgB3YlC/4B5k0F"
    "rMAb2IETsAIn4ApswBE4gS/4ADswA3fgBmzBGfiBTfgbcDc3wBdoAFf4Ayb0B2boD8zQH5ihPzBD"
    "f2CG/sAM/YEp+gM06A/coD9wg/7ADfoDN+gP3KA/cIP+wA36Azdob8B+NE7x/wAvYk4a9QAAAABJ"
    "RU5ErkJggg==";



NotificationWindow::NotificationWindow(wxWindow* parent,
                                       const wxString& title,
                                       const wxString& slackChannel,
                                       const wxString& sender,
                                       const wxString& time,
                                       const wxString& message)
    : wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxSTAY_ON_TOP)
{
    // --- Load Bitmaps from Files ---
    wxBitmap closeBitmap(wxT("images/close.png"), wxBITMAP_TYPE_PNG);
    wxBitmap userBitmap(wxT("images/exclamation.png"), wxBITMAP_TYPE_PNG);

    // --- Error Checking ---
    if (!closeBitmap.IsOk() || !userBitmap.IsOk())
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
    titleBarPanel->SetBackgroundColour(wxColour(82, 38, 83));
    wxBoxSizer* titleBarSizer = new wxBoxSizer(wxHORIZONTAL);

    wxPanel* contentPanel = new wxPanel(backgroundPanel);
    contentPanel->SetBackgroundColour(*wxWHITE);
    wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);

    // --- Populate Title Bar ---
    wxStaticText* slackIcon = new wxStaticText(titleBarPanel, wxID_ANY, wxT("#"));
    slackIcon->SetForegroundColour(*wxWHITE);
    slackIcon->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    wxStaticText* titleText = new wxStaticText(titleBarPanel, wxID_ANY, title);
    titleText->SetForegroundColour(*wxWHITE);
    titleText->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Segoe UI"));

    wxBitmapButton* closeButton = new wxBitmapButton(titleBarPanel, wxID_ANY, closeBitmap, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    closeButton->SetBackgroundColour(wxColour(82, 38, 83));

    titleBarSizer->Add(slackIcon, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 10);
    titleBarSizer->Add(titleText, 1, wxALIGN_CENTER_VERTICAL);
    titleBarSizer->Add(closeButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    titleBarPanel->SetSizer(titleBarSizer);

    // --- Populate Content Area ---
    wxStaticBitmap* userIcon = new wxStaticBitmap(contentPanel, wxID_ANY, userBitmap);
    wxPanel* textPanel = new wxPanel(contentPanel);
    wxBoxSizer* textSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* channelText = new wxStaticText(textPanel, wxID_ANY, slackChannel);
    channelText->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Segoe UI"));

    wxBoxSizer* senderSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* senderText = new wxStaticText(textPanel, wxID_ANY, sender);
    senderText->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Segoe UI"));
    wxStaticText* timeText = new wxStaticText(textPanel, wxID_ANY, time);
    timeText->SetForegroundColour(*wxLIGHT_GREY);
    timeText->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Segoe UI"));
    senderSizer->Add(senderText, 0, wxRIGHT, 5);
    senderSizer->Add(timeText, 0);

    wxStaticText* messageText = new wxStaticText(textPanel, wxID_ANY, message);
    messageText->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Segoe UI"));
    messageText->Wrap(250);

    textSizer->Add(channelText, 0, wxBOTTOM, 5);
    textSizer->Add(senderSizer, 0, wxBOTTOM, 5);
    textSizer->Add(messageText, 1, wxEXPAND);
    textPanel->SetSizer(textSizer);

    contentSizer->Add(userIcon, 0, wxALL, 10);
    contentSizer->Add(textPanel, 1, wxTOP | wxBOTTOM | wxRIGHT, 10);
    contentPanel->SetSizer(contentSizer);

    // --- Final Layout (Corrected) ---
    mainSizer->Add(titleBarPanel, 0, wxEXPAND);
    mainSizer->Add(contentPanel, 1, wxEXPAND);
    
    // Set the sizer for the main panel that contains all the controls
    backgroundPanel->SetSizer(mainSizer);

    // Create a new sizer for the frame itself
    wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
    // Add the main panel to the frame's sizer, making it expand to fill the frame
    frameSizer->Add(backgroundPanel, 1, wxEXPAND);
    
    // Set the sizer for the frame and fit the frame to the sizer's content
    this->SetSizerAndFit(frameSizer);


    // --- Bind Events ---
    titleBarPanel->Bind(wxEVT_LEFT_DOWN, &NotificationWindow::OnMouseDown, this);
    titleBarPanel->Bind(wxEVT_LEFT_UP, &NotificationWindow::OnMouseUp, this);
    titleBarPanel->Bind(wxEVT_MOTION, &NotificationWindow::OnMouseMove, this);
    titleText->Bind(wxEVT_LEFT_DOWN, &NotificationWindow::OnMouseDown, this);
    titleText->Bind(wxEVT_LEFT_UP, &NotificationWindow::OnMouseUp, this);
    titleText->Bind(wxEVT_MOTION, &NotificationWindow::OnMouseMove, this);
    slackIcon->Bind(wxEVT_LEFT_DOWN, &NotificationWindow::OnMouseDown, this);
    slackIcon->Bind(wxEVT_LEFT_UP, &NotificationWindow::OnMouseUp, this);
    slackIcon->Bind(wxEVT_MOTION, &NotificationWindow::OnMouseMove, this);
    closeButton->Bind(wxEVT_BUTTON, &NotificationWindow::OnCloseButtonClick, this);

    wxRect screenRect = wxGetClientDisplayRect();
    wxSize windowSize = GetSize();
    SetPosition(wxPoint(screenRect.width - windowSize.x - 20, screenRect.height - windowSize.y - 40));
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