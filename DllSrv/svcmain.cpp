#include "pch.h"
#include "svcmain.h"


//Õª×Ô£ºXPSP1\NT\base\screg\sc\svchost\sample\server\svcmain.c


//////////////////////////////////////////////////////////////////////////////////////////////////


SERVICE_STATUS_HANDLE   g_hStatus;
SERVICE_STATUS          g_status;
HANDLE                  g_hShutdownEvent;


VOID UpdateServiceStatus(DWORD   dwState)
{
    g_status.dwCurrentState = dwState;
    SetServiceStatus(g_hStatus, &g_status);
}


VOID WINAPI ServiceHandler(DWORD dwOpcode)
// ServiceHandler - Called by the service controller at various times.
//
// type of LPHANDLER_FUNCTION
{
    switch (dwOpcode) {
    case SERVICE_CONTROL_STOP:
        UpdateServiceStatus(SERVICE_STOP_PENDING);

        // set an event or otherwise signal that we are to quit.
        // e.g. RpcMgmtStopServerListening
        SetEvent(g_hShutdownEvent);
        break;
    case SERVICE_CONTROL_PAUSE:
    case SERVICE_CONTROL_CONTINUE:
    case SERVICE_CONTROL_INTERROGATE:
    case SERVICE_CONTROL_SHUTDOWN:
    default:
        // This may not be need, but refresh our status to the service controller.
        SetServiceStatus(g_hStatus, &g_status);
        break;
    }
}


EXTERN_C
__declspec(dllexport)
VOID WINAPI ServiceMain(DWORD argc, PWSTR argv[])
// ServiceMain - Called by svchost when starting this service.
//
// type of LPSERVICE_MAIN_FUNCTIONW
{
    //DebugBreak();

    // Since we run in svchost.exe, we must have the 'share process' bit set.
    ZeroMemory(&g_status, sizeof(g_status));
    g_status.dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
    g_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    // Register the service control handler.
    g_hStatus = RegisterServiceCtrlHandler(TEXT("dllsrv"), ServiceHandler);
    if (g_hStatus) {
        // Immediately report that we are running.  All non-essential
        // initialization is deferred until we are called by clients to do some work.
        UpdateServiceStatus(SERVICE_RUNNING);

        g_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        _ASSERTE(g_hShutdownEvent);

        // Setup RPC and call RpcServerListen.
        //
        // RpcMgmtWaitServerListen (NULL);

        WaitForSingleObject(g_hShutdownEvent, INFINITE);

        UpdateServiceStatus(SERVICE_STOPPED);
    } else {
        //DbgPrint("RegisterServiceCtrlHandler failed!  %d\n", GetLastError());
    }
}
