#include "logger.h"
#include <wx/file.h>
#include <wx/filename.h>

static const wxString LOG_FILE = "notifications.log";
static const wxString OLD_LOG_FILE = "notifications_older.log";
static const long MAX_LOG_SIZE = 1024 * 1024; // 1 MB
static bool loggingEnabled = true;

void RotateLogs() {
    if (!loggingEnabled) return;

    wxFileName logFile(LOG_FILE);
    if (logFile.Exists() && logFile.GetSize() > MAX_LOG_SIZE) {
        if (wxFileName::Exists(OLD_LOG_FILE)) {
            if (!wxRemoveFile(OLD_LOG_FILE)) {
                loggingEnabled = false;
                return;
            }
        }
        if (!wxRenameFile(LOG_FILE, OLD_LOG_FILE)) {
            loggingEnabled = false;
            return;
        }
    }
}

void LogMessage(const wxString& message) {
    if (!loggingEnabled) return;

    RotateLogs();
    if (!loggingEnabled) return;

    wxFile file;
    if (!file.Open(LOG_FILE, wxFile::write_append)) {
        loggingEnabled = false;
        return;
    }

    wxDateTime now = wxDateTime::Now();
    file.Write(now.FormatISOCombined(' ') + " - " + message + "\n");
    file.Close();
}