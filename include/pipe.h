#pragma once
#include <windows.h>
#include <string>

class Pipe {
public:
    Pipe(const std::wstring& name, DWORD openMode, DWORD pipeMode = PIPE_TYPE_BYTE | PIPE_WAIT, DWORD maxInstances = 1, DWORD outBufferSize = 1024 * 16, DWORD inBufferSize = 1024 * 16);
    ~Pipe();

    bool Connect();
    bool Read(char* buffer, DWORD bufferSize, DWORD& bytesRead);
    bool Write(const char* buffer, DWORD bufferSize, DWORD& bytesWritten);
    void Disconnect();
    bool IsValid() const;

private:
    HANDLE m_handle;
};