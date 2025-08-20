#include "logger.h"
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/datetime.h>
#include <wx/filefn.h>
#include <wx/log.h>

static const long MAX_LOG_SIZE = 1024 * 1024; // 1 MB
static bool loggingEnabled = true;

// Build a full path in a safe per-user directory
static wxString GetLogFilePath(const wxString& name)
{
    wxString dir = wxStandardPaths::Get().GetUserLocalDataDir();

    // Ensure directory exists
    if (!wxDirExists(dir)) {
        wxFileName::Mkdir(dir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
    }

    return dir + wxFileName::GetPathSeparator() + name;
}

void RotateLogs()
{
    if (!loggingEnabled) return;

    wxString logFile = GetLogFilePath("notifications.log");
    wxString oldLogFile = GetLogFilePath("notifications_older.log");

    wxFileName fn(logFile);
    if (fn.Exists() && fn.GetSize() > MAX_LOG_SIZE) {
        if (wxFileName::Exists(oldLogFile)) {
            if (!wxRemoveFile(oldLogFile)) {
                loggingEnabled = false;
                return;
            }
        }
        if (!wxRenameFile(logFile, oldLogFile)) {
            loggingEnabled = false;
            return;
        }
    }
}

void LogMessage(const wxString& message)
{
    if (!loggingEnabled) return;

    RotateLogs();
    if (!loggingEnabled) return;

    wxString logPath = GetLogFilePath("notifications.log");
    wxFile file;
    if (!file.Open(logPath, wxFile::write_append)) {
        loggingEnabled = false;
        wxLogError("Failed to open log file: %s", logPath);
        return;
    }

    wxDateTime now = wxDateTime::Now();
    wxString line = now.FormatISOCombined(' ') + " - " + message + "\n";

    if (!file.Write(line)) {
        loggingEnabled = false;
        wxLogError("Failed to write log line to: %s", logPath);
    }

    file.Close();
}