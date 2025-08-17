#include <winsock2.h>
#include "pipe.h"
#include "logger.h"


Pipe::Pipe(const std::wstring& name, DWORD openMode, DWORD pipeMode, DWORD maxInstances, DWORD outBufferSize, DWORD inBufferSize) {

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = FALSE;

    // Create a security descriptor that allows access to everyone (NULL DACL)
    PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(pSD, TRUE, (PACL)NULL, FALSE); // Allows all access
    sa.lpSecurityDescriptor = pSD;


    m_handle = CreateNamedPipeW(
        name.c_str(),
        openMode,
        pipeMode,
        maxInstances,
        outBufferSize,
        inBufferSize,
        NMPWAIT_USE_DEFAULT_WAIT,
        &sa);

    if (m_handle == INVALID_HANDLE_VALUE) {
        LogMessage(L"Failed to create named pipe: " + name);
    }
}

Pipe::~Pipe() {
    if (IsValid()) {
        CloseHandle(m_handle);
    }
}

bool Pipe::Connect() {
    if (!IsValid()) return false;
    return ConnectNamedPipe(m_handle, NULL) ? true : (GetLastError() == ERROR_PIPE_CONNECTED);
}

bool Pipe::Read(char* buffer, DWORD bufferSize, DWORD& bytesRead) {
    if (!IsValid()) return false;
    return ReadFile(m_handle, buffer, bufferSize, &bytesRead, NULL);
}

bool Pipe::Write(const char* buffer, DWORD bufferSize, DWORD& bytesWritten) {
    if (!IsValid()) return false;
    return WriteFile(m_handle, buffer, bufferSize, &bytesWritten, NULL);
}

void Pipe::Disconnect() {
    if (IsValid()) {
        DisconnectNamedPipe(m_handle);
    }
}

bool Pipe::IsValid() const {
    return m_handle != INVALID_HANDLE_VALUE;
}