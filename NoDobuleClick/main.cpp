#define _WIN32_WINNT 0x0400
#pragma comment( lib, "user32.lib" )
 

#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <string.h>
HHOOK hMouseHook;
 
double timerFrequency;
unsigned __int64 lclick = 0;
unsigned __int64 rclick = 0;
unsigned char lblocked = 0;
unsigned char rblocked = 0;
double timeoutl, timeoutr;

__declspec(dllexport) LRESULT CALLBACK KeyboardEvent (int nCode, WPARAM wParam, LPARAM lParam)
{
    MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
    if (pMouseStruct != NULL)
	{
		if (timeoutl > 0 && wParam == WM_LBUTTONDOWN)
		{
			unsigned __int64 curTime;
			QueryPerformanceCounter((LARGE_INTEGER *)&curTime);
			double timeDifferenceInMilliseconds = ((curTime-lclick) * timerFrequency);
			lclick = curTime;

			if (timeDifferenceInMilliseconds < timeoutl)
			{
				printf("Left Button DOWN Blocked %.3f < %.3f\n", timeDifferenceInMilliseconds, timeoutl);
				lblocked = 1;
				return 1;
			}	
			lblocked = 0;
		}
		else if (timeoutl > 0 && wParam == WM_LBUTTONUP && lblocked == 1)
		{
			printf("Left Button UP Blocked\n");
			return 1;
		}
		else if (timeoutr > 0 && wParam == WM_RBUTTONDOWN)
		{
			unsigned __int64 curTime;
			QueryPerformanceCounter((LARGE_INTEGER *)&curTime);
			double timeDifferenceInMilliseconds = ((curTime-rclick) * timerFrequency);
			rclick = curTime;

			if (timeDifferenceInMilliseconds < timeoutr)
			{
				printf("Right Button DOWN Blocked %.3f < %.3f\n", timeDifferenceInMilliseconds, timeoutr);
				rblocked = 1;
				return 1;
			}	
			rblocked = 0;
		}
		else if (timeoutr > 0 && wParam == WM_RBUTTONUP && rblocked == 1)
		{
			printf("Right Button UP Blocked\n");
			return 1;
		}
	}
    return CallNextHookEx(hMouseHook, nCode,wParam,lParam);
}
 
void MessageLoop()
{
    MSG message;
    while (GetMessage(&message,NULL,0,0)) {
        TranslateMessage( &message );
        DispatchMessage( &message );
    }
}
 
DWORD WINAPI MyMouseLogger(LPVOID lpParm)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    if (!hInstance) hInstance = LoadLibrary((LPCSTR) lpParm); 
    if (!hInstance) return 1;
 
    hMouseHook = SetWindowsHookEx (  
        WH_MOUSE_LL,
        (HOOKPROC) KeyboardEvent,  
        hInstance,                 
        NULL                       
        );
    MessageLoop();
    UnhookWindowsHookEx(hMouseHook);
    return 0;
}
 
int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
)
{
    HANDLE hThread;
    DWORD dwThread;

	unsigned __int64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	timerFrequency = (1.0/freq);
	timeoutl = timeoutr = 0;

	LPWSTR *argv;
    int argc;
    argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	 for (int i = 1; i < argc; i++)
	 {
            if (i + 1 != argc) 
			{
				if (!_wcsicmp(argv[i], L"-l"))
				{
					timeoutl = _wtof(argv[i + 1]);
                }
				else if (!_wcsicmp(argv[i], L"-r"))
				{
                    timeoutr = _wtof(argv[i + 1]);
                }
			}
	 }
	 if (timeoutl == timeoutr && timeoutl == 0)
	 {
		 timeoutl = timeoutr = 0.1;
	 }


    hThread = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE) MyMouseLogger, (LPVOID) argv[0], NULL, &dwThread);
    if (hThread)
        return WaitForSingleObject(hThread,INFINITE);
    else return 1;
 
}