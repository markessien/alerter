#ifndef EVENTS_H
#define EVENTS_H

#include <wx/event.h>

wxDECLARE_EVENT(wxEVT_LOGIN_REQUEST, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_LOGIN_SUCCESS, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_LOGIN_FAILURE, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_WEBSOCKET_ERROR, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_SHOW_NOTIFICATION, wxCommandEvent);

#endif // EVENTS_H