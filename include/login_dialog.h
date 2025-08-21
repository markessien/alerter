#pragma once

#include <wx/wx.h>
#include <wx/dialog.h>

class LoginDialog : public wxDialog
{
public:
    LoginDialog(wxWindow* parent, wxWindowID id = wxID_ANY,
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

    wxDECLARE_EVENT_TABLE();
};