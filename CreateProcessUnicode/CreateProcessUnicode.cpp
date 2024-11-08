#include <windows.h>
#include <iostream>

void CaptureProcessOutput() {
    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    HANDLE hChildStdOutRd, hChildStdOutWr;

    // Create a pipe for stdout
    if (!CreatePipe(&hChildStdOutRd, &hChildStdOutWr, &saAttr, 0)) {
        std::wcerr << L"CreatePipe failed!" << std::endl;
        return;
    }

    // Ensure the read handle is not inherited
    SetHandleInformation(hChildStdOutRd, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdOutput = hChildStdOutWr;
    si.hStdError = hChildStdOutWr;


    wchar_t command[] = L"/C powershell -Command Get-LocalUser";

    if (!CreateProcessW(
        L"C:\\Windows\\system32\\cmd.exe",
        command,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &si,
        &pi)) {
        std::wcerr << L"CreateProcess failed!" << std::endl;
        CloseHandle(hChildStdOutRd);
        CloseHandle(hChildStdOutWr);
        return;
    }

    // Close the write end of the pipe to signal end of input
    CloseHandle(hChildStdOutWr);

    // Read the output from the process
    DWORD bytesRead;
    CHAR buffer[4096];
    while (ReadFile(hChildStdOutRd, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        // Null-terminate the buffer to safely convert it
        buffer[bytesRead] = '\0';

        // Convert to UTF-16 for output
        UINT codePage = GetConsoleOutputCP();
        if (codePage == CP_UTF8) {
            int wideCharLen = MultiByteToWideChar(CP_UTF8, 0, buffer, bytesRead, NULL, 0);
            wchar_t* wideCharBuffer = new wchar_t[wideCharLen + 1];
            MultiByteToWideChar(CP_UTF8, 0, buffer, bytesRead, wideCharBuffer, wideCharLen);
            wideCharBuffer[wideCharLen] = L'\0';  // Null-terminate the string
            std::wcout << wideCharBuffer;
            delete[] wideCharBuffer;
        }
        else {
            std::wcout << L"Console is not UTF-8. Output may not display correctly." << std::endl;
        }
    }

    // Close process handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hChildStdOutRd);
}

int main() {
    if (SetConsoleOutputCP(CP_UTF8)) {
        std::cout << "Code page set to UTF-8 (65001)." << std::endl;
    }
    else {
        std::cerr << "Failed to set code page." << std::endl;
    }
    CaptureProcessOutput();
    return 0;
}
