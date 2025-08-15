#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/mstream.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>


class NotificationWindow : public wxFrame
{
public:
    NotificationWindow(wxWindow* parent,
                       const wxString& title,
                       const wxString& slackChannel,
                       const wxString& sender,
                       const wxString& time,
                       const wxString& message);

private:
    // Event handlers for dragging the borderless window
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);

    // Event handler for the custom close button
    void OnCloseButtonClick(wxCommandEvent& event);

    wxPoint m_delta; // Position offset used for dragging
};