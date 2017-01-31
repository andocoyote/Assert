/*
Simple program to explore ASSERT nad VERIFY macros by closing a handle twice
while running the debug build
*/
#include "precompiled.h"
#include <windows.h>
#include <crtdbg.h>
#include <stdio.h>

#define ASSERT _ASSERTE

#ifdef _DEBUG
#define ISDEBUG 1
#define VERIFY ASSERT
#else
#define ISDEBUG 0
#define VERIFY(expression) (expression)
#endif

struct LastException
{
    DWORD result;
    LastException() : result { GetLastError()} {}
};

struct ManualResetEvent
{
    HANDLE m_handle;
    
    ManualResetEvent()
    {
        printf("ManualResetEvent::Constructor: creating event.\n");
        m_handle = CreateEvent(nullptr,
                               true,
                               false,
                               nullptr);
                               
        if(!m_handle)
        {
            printf("ManualResetEvent::Constructor: throwing LastException().\n");
            throw LastException();
        }
    }
    
    ~ManualResetEvent()
    {
        printf("ManualResetEvent::Destructor: calling CloseHandle.\n");
        VERIFY(CloseHandle(m_handle));
    }
};

auto main() -> int
{
    printf("ISDEBUG: %d\n", ISDEBUG);
    printf("Main: creating e.\n");
    auto e = ManualResetEvent {};
    
    printf("Main: creating e2.\n");
    auto e2 = e;
    
    // When the destructor for e2 is called, CloseHandle will return 0 since the
    // handle is already closed, which will trigger VERIFY to ASSERT in debug builds.
    // In release builds, VERIFY does not ASSERT
}
