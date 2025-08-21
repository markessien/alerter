#pragma once

#include <wx/wx.h>
#include <wx/dialog.h>
#include "telex.h" // Include Telex header

class LoginDialog : public wxDialog
{
public:
    LoginDialog(wxWindow* parent, Telex* telex, wxWindowID id = wxID_ANY,
                const wxString& title = wxT("Login"),
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_DIALOG_STYLE);

    wxString GetUsername() const;
    wxString GetPassword() const;

private:
    void OnLogin(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    wxTextCtrl* m_usernameCtrl;
    wxTextCtrl* m_passwordCtrl;
    wxButton* m_loginBtn;
    wxButton* m_cancelBtn;
    Telex* m_telex; // Pointer to Telex instance

    wxDECLARE_EVENT_TABLE();
};