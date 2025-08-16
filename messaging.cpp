#include <wx/wx.h>
#include "messaging.h"

Messaging::Messaging() : wxThread(wxTHREAD_DETACHED) {}

wxThread::ExitCode Messaging::Entry() {
    // Allow the main app to initialize
    wxSleep(2);

    HANDLE hPipe = CreateNamedPipe(
        TEXT("\\\\.\\pipe\\telex-notifications"),
        PIPE_ACCESS_INBOUND,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1, // nMaxInstances
        1024 * 16, // nOutBufferSize
        1024 * 16, // nInBufferSize
        NMPWAIT_USE_DEFAULT_WAIT, // nDefaultTimeOut
        NULL // lpSecurityAttributes
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        wxMessageBox("Failed to create named pipe", "Error", wxOK | wxICON_ERROR);
        return (wxThread::ExitCode)0;
    }

    while (!TestDestroy()) {
        // Wait for the client to connect; if it succeeds,
        // the function returns a nonzero value. If the function
        // returns zero, GetLastError returns ERROR_PIPE_CONNECTED.
        bool connected = ConnectNamedPipe(hPipe, NULL) ?
            true : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (connected) {
            char buffer[1024];
            DWORD dwRead;
            // Read client requests from the pipe.
            if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL)) {
                buffer[dwRead] = '\0';
                wxMessageBox(wxString::FromUTF8(buffer), "New Data", wxOK);
            }
        }

        // Disconnect the pipe to allow another client to connect.
        DisconnectNamedPipe(hPipe);
    }

    CloseHandle(hPipe);
    return (wxThread::ExitCode)0;
}