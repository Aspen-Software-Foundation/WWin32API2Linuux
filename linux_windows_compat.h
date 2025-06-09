#ifndef LINUX_WINDOWS_COMPAT_H
#define LINUX_WINDOWS_COMPAT_H

// Core includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
#include <wchar.h>
#include <locale.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>



// Basic Windows-style types
typedef int BOOL;
typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t BYTE;
typedef void* HANDLE;
typedef const char* LPCSTR;
typedef char* LPSTR;
typedef const wchar_t* LPCWSTR;
typedef wchar_t* LPWSTR;
typedef void* LPVOID;
typedef const void* LPCVOID;

#ifndef NULL
#define NULL ((void*)0)
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


static inline void Delay(DWORD milliseconds) {
    usleep(milliseconds * 1000); // Example: Sleep(1000); // Sleep for 1 second
}

static inline DWORD GetTickCount() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (DWORD)((ts.tv_sec * 1000) + (ts.tv_nsec / 1000000)); //Example: printf("Ticks: %u\n", GetTickCount());
}


#define lstrcpyA strcpy
#define lstrcpyW wcscpy
#define lstrcatA strcat
#define lstrcatW wcscat
#define lstrlenA strlen
#define lstrlenW wcslen


static inline int MessageBoxA(void* hwnd, const char* text, const char* title, unsigned int type) {
    (void)hwnd;  //Example: MessageBoxA(NULL, "Hello, World!", "Test", 0);


    if (access("/usr/bin/zenity", X_OK) == 0) {
        char command[1024];
        snprintf(command, sizeof(command), "zenity --info --title=\"%s\" --text=\"%s\"", title, text);
        return system(command);
    } else {
        // Fallback to console output
        printf("[MessageBox - %s] %s\n", title ? title : "Untitled", text);
        return 0;
    }
}
static inline int MessageBoxW(void* hwnd, LPCWSTR text, LPCWSTR title, unsigned int type) {
    fwprintf(stdout, L"[MessageBox - %ls] %ls\n", title, text); //Example: MessageBoxW(NULL, L"Hello, World!", L"Test", 0);
    return 0;
}


static inline int wsprintfA(LPSTR buffer, LPCSTR format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vsprintf(buffer, format, args);  //Example: wsprintfA(buffer, "Hello %s", "World");
    va_end(args);
    return ret;
}

static inline int wsprintfW(LPWSTR buffer, LPCWSTR format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vswprintf(buffer, 1024, format, args); // 1024 char safety buffer
    va_end(args);
    return ret;
}

#define wvsprintfA vsprintf
#define wvsprintfW vswprintf

#define wprintfW wprintf
#define wprintfA printf

#define wsprintf sprintf

// ==== Threading (basic) ====
typedef struct {
    pthread_t thread;
    LPVOID (*start_routine)(LPVOID);
    LPVOID arg;
} WIN_THREAD;



#define WAIT_OBJECT_0 0
#define INFINITE 0xFFFFFFFF

static inline DWORD WaitForSingleObject(HANDLE h, DWORD millis) {
    WIN_THREAD* win_thread = (WIN_THREAD*)h;
    if (millis == INFINITE) {
        pthread_join(win_thread->thread, NULL);
        free(win_thread);
        return WAIT_OBJECT_0;
    } else {
        // Timeout not yet implemented
        pthread_join(win_thread->thread, NULL);
        free(win_thread);
        return WAIT_OBJECT_0;
    }
}

#endif // LINUX_WINDOWS_COMPAT_H
