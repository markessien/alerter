#include "login_dialog.h"
#include <wx/sizer.h>
#include <wx/stattext.h>

enum
{
    ID_USERNAME = wxID_HIGHEST + 1,
    ID_PASSWORD,
    ID_LOGIN_BTN,
    ID_CANCEL_BTN
};

wxBEGIN_EVENT_TABLE(LoginDialog, wxDialog)
    EVT_BUTTON(ID_LOGIN_BTN, LoginDialog::OnLogin)
    EVT_BUTTON(ID_CANCEL_BTN, LoginDialog::OnCancel)
wxEND_EVENT_TABLE()

LoginDialog::LoginDialog(wxWindow* parent, Telex* telex, wxWindowID id, const wxString& title,
                        const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style),
      m_telex(telex) // Initialize m_telex
{
    // Create controls
    m_usernameCtrl = new wxTextCtrl(this, ID_USERNAME, wxEmptyString,
                                   wxDefaultPosition, wxSize(200, -1));
    m_passwordCtrl = new wxTextCtrl(this, ID_PASSWORD, wxEmptyString,
                                   wxDefaultPosition, wxSize(200, -1),
                                   wxTE_PASSWORD);

    m_loginBtn = new wxButton(this, ID_LOGIN_BTN, wxT("Login"));
    m_cancelBtn = new wxButton(this, ID_CANCEL_BTN, wxT("Cancel"));

    // Set default button
    m_loginBtn->SetDefault();

    // Create sizers for layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 2, 5, 5);

    // Add labels and controls to grid
    gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Username:")), 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_usernameCtrl, 1, wxEXPAND);

    gridSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Password:")), 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(m_passwordCtrl, 1, wxEXPAND);

    // Add grid to main sizer
    mainSizer->Add(gridSizer, 0, wxALL | wxEXPAND, 10);

    // Add button sizer
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(m_loginBtn, 0, wxRIGHT, 5);
    buttonSizer->Add(m_cancelBtn, 0, wxLEFT, 5);

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizer(mainSizer);
    mainSizer->Fit(this);
    mainSizer->SetSizeHints(this);

    // Center the dialog
    Centre();
}

wxString LoginDialog::GetUsername() const
{
    return m_usernameCtrl->GetValue();
}

wxString LoginDialog::GetPassword() const
{
    return m_passwordCtrl->GetValue();
}

void LoginDialog::OnLogin(wxCommandEvent& event)
{
    if (GetUsername().IsEmpty() || GetPassword().IsEmpty())
    {
        wxMessageBox(wxT("Please enter both username and password."),
                     wxT("Login Error"), wxICON_ERROR);
        return;
    }

    // For now, just show a message and close
    if (m_telex->login(GetUsername().ToStdString(), GetPassword().ToStdString()))
    {
        wxMessageBox(wxT("Login successful!"), wxT("Login"), wxICON_INFORMATION);
        EndModal(wxID_OK);
    }
    else
    {
        wxMessageBox(wxT("Login failed. Please check your username and password."),
                     wxT("Login Error"), wxICON_ERROR);
    }
}

void LoginDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}