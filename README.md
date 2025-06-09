# Windows API Compatibility Layer for Linux

Overview

This document describes how to use the linux_windows_compat.h header — a Windows API compatibility layer that allows you to write code using Win32-style functions and types, but run it on Linux systems.

It provides support for basic types, timing functions, message boxes, string manipulation, formatted output, and basic threading simulation.



The following typedefs are defined to mirror Win32 types:


**Description:**
BOOL

**Integer boolean (int):**
DWORD

**32-bit unsigned integer:**
WORD

**16-bit unsigned integer:**
BYTE

**8-bit unsigned integer:**
HANDLE

**Generic pointer (void):**
LPCSTR

**Pointer to constant ANSI string:**
LPSTR

**Pointer to modifiable ANSI string:**
LPCWSTR

**Pointer to constant wide string:**
LPWSTR

**Pointer to modifiable wide string:**
LPVOID

**Pointer to any type (void):**
LPCVOID

Constant pointer to any type

# Constants:

NULL, TRUE, FALSE are also defined for compatibility.

# Timing Functions

void Delay(DWORD milliseconds)

**Description:**
Delays execution for the specified number of milliseconds.

**Usage:**
Delay(1000); // Sleep for 1 second
----------------------------------------------------------------

DWORD GetTickCount()

**Description:**
Returns the number of milliseconds since the system started.

**Usage:**
DWORD ticks = GetTickCount();
printf("System uptime: %u ms\n", ticks);
----------------------------------------------------------------

# Message Boxes
int MessageBoxA(void* hwnd, const char* text, const char* title, unsigned int type)

**Description:**
Displays a simple message box using zenity if available, or prints to terminal otherwise.

Usage:
MessageBoxA(NULL, "Operation complete", "MyApp", 0);
----------------------------------------------------------------

int MessageBoxW(void* hwnd, LPCWSTR text, LPCWSTR title, unsigned int type)

Description:
Wide-character version of MessageBoxA. Outputs to terminal using fwprintf.

Usage:
MessageBoxW(NULL, L"Operation complete", L"MyApp", 0);

Note: Set locale in your main() to ensure proper wide string rendering:
```setlocale(LC_ALL, "");```
----------------------------------------------------------------
Formatted Output:
int wsprintfA(LPSTR buffer, LPCSTR format, ...)

Description:
ANSI version of sprintf with variable arguments.

Usage:
char buffer[128];
wsprintfA(buffer, "Hello %s", "World");
----------------------------------------------------------------
int wsprintfW(LPWSTR buffer, LPCWSTR format, ...)

Description:
Wide-character version of sprintf.

Usage:
wchar_t buffer[128];

wsprintfW(buffer, L"Hello %ls", L"World");
----------------------------------------------------------------
# Macros

*String Functions*
ANSI (A) and Wide (W) versions are provided:
Macro	Equivalent function
lstrcpyA	strcpy
lstrcpyW	wcscpy
lstrcatA	strcat
lstrcatW	wcscat
lstrlenA	strlen
lstrlenW	wcslen




# Threading

Basic threading is simulated using pthread.

Internal Type:

typedef struct {
    pthread_t thread;
    LPVOID (*start_routine)(LPVOID);
    LPVOID arg;
} WIN_THREAD;

DWORD WaitForSingleObject(HANDLE h, DWORD millis)

Description:
Waits for the thread to complete. INFINITE means wait indefinitely.

Usage:
HANDLE hThread = CreateThread(...); // Not implemented here
----------------------------------------------------------------
WaitForSingleObject(hThread, INFINITE);

Note: All waits are treated as infinite; timeout handling is not implemented.



WAIT_OBJECT_0:
Return value on success

INFINITE:
Used to block forever

Full Example:

#include "linux_windows_compat.h"

int main() {
    setlocale(LC_ALL, "");

    MessageBoxA(NULL, "Welcome to Linux!", "WinCompat", 0);
    MessageBoxW(NULL, L"Hello, \u4E16\u754C!", L"WideBox", 0);

    DWORD t = GetTickCount();
    printf("Ticks: %u ms\n", t);

    Delay(1000);
    return 0;
}

# Requirements

Linux-based system

zenity (for GUI message boxes)

Install with: sudo apt install zenity

Locale setup (setlocale(LC_ALL, "");) recommended for wide-string support

# License

This compatibility header is licensed under the GPLv3 license. For more information, go to https://www.gnu.org/licenses/gpl-3.0.en.html
