#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/mstream.h>
#include <wx/graphics.h>


#include "notification_content.h"
#include "images.h"

NotificationContent::NotificationContent(wxWindow* parent,
    const wxString& channel,
    const wxString& sender,
    const wxString& time,
    const wxString& message,
    const wxString& iconPath,
    int width,
    int top,
    int left)
    : wxPanel(parent, wxID_ANY, wxPoint(left, top))
{
    SetBackgroundColour(*wxWHITE);
    wxBoxSizer* contentBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    wxImage alertUserImage;
    if (wxFileExists(iconPath))
    {
        alertUserImage.LoadFile(iconPath, wxBITMAP_TYPE_ANY);
    }
    else
    {
        wxMemoryInputStream stream(_aclogo48, _aclogo48_size);

        wxImage image;
        if (!image.LoadFile(stream, wxBITMAP_TYPE_PNG))
        {
            // Handle the error if the image data is invalid
            wxLogError("Failed to load embedded PNG image.");
        }
        
        alertUserImage = image;

    }

    alertUserImage.Rescale(48, 48);
    wxBitmap alertUserBitmap(alertUserImage);
    wxStaticBitmap* alertUserIcon = new wxStaticBitmap(this, wxID_ANY, alertUserBitmap);

    wxBoxSizer* textSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* senderSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* senderText = new wxStaticText(this, wxID_ANY, sender);
    senderText->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Segoe UI"));
    

    wxStaticText* timeText = new wxStaticText(this, wxID_ANY, time);
    timeText->SetForegroundColour(wxColour(160, 160, 160));
    timeText->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Segoe UI"));
    
    senderSizer->Add(senderText, 0, wxALIGN_CENTER_VERTICAL);
    if (!channel.IsEmpty())
    {
        wxStaticText* channelText = new wxStaticText(this, wxID_ANY, " in " + channel);
        channelText->SetForegroundColour(wxColour(160, 160, 160));
        channelText->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Segoe UI"));
        senderSizer->Add(channelText, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
    }
    senderSizer->AddStretchSpacer(1);
    senderSizer->Add(timeText, 0, wxALIGN_CENTER_VERTICAL);

    wxStaticText* messageText = new wxStaticText(this, wxID_ANY, message);
    messageText->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Segoe UI"));
    messageText->Wrap(width - 80);

    textSizer->Add(senderSizer, 0, wxEXPAND);
    textSizer->Add(messageText, 1, wxEXPAND | wxTOP, 5);

    contentBoxSizer->Add(alertUserIcon, 0, wxALIGN_TOP | wxALL, 10);
    contentBoxSizer->Add(textSizer, 1, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 10);

    SetSizer(contentBoxSizer);
}