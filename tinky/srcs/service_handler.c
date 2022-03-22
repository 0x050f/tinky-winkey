#include "tinky.h"

void		report_service_status(DWORD dw_status)
{
	g_service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_service_status.dwControlsAccepted = 0;
	if (dw_status == SERVICE_START_PENDING)
		g_service_status.dwControlsAccepted = 0;
	else
		g_service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	g_service_status.dwCurrentState = dw_status;
	g_service_status.dwWin32ExitCode = 0;
	g_service_status.dwServiceSpecificExitCode = 0;
	g_service_status.dwCheckPoint = 0;
	if (SetServiceStatus (g_status_handle, &g_service_status) == FALSE)
		printf("%s() failed, error: %ld\n", "SetServiceStatus", GetLastError());
}

DWORD WINAPI service_control_handler(DWORD dw_ctrl)
{
    switch (dw_ctrl)
	{
		case SERVICE_CONTROL_STOP :
			kill_process_id(find_process_id(EXEC_NAME));
			report_service_status(SERVICE_STOPPED);
			break ;
		default:
			;
	}
	return NO_ERROR;
}

LPSERVICE_MAIN_FUNCTIONA service_main(DWORD ac, LPSTR *av)
{
	if (ac <= 1)
		return (NULL);
	g_status_handle = RegisterServiceCtrlHandler(SERVICE_NAME, service_control_handler);
	if (!g_status_handle)
	{
		printf("%s() failed, error: %ld\n", "RegisterServiceCtrlHandler", GetLastError());
		return (NULL);
	}
	report_service_status(SERVICE_START_PENDING);
	g_service_stop_event = CreateEvent (NULL, TRUE, FALSE, NULL);
	if (g_service_stop_event == NULL) 
    {
		printf("%s() failed, error: %ld\n", "CreateEvent", GetLastError());
		return (NULL);
	}
	report_service_status(SERVICE_RUNNING);
	HANDLE dup_token_handle = NULL;
	HANDLE token_handle = NULL;
	DWORD pid = find_process_id("winlogon.exe");
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, true, pid);
	STARTUPINFO info = {sizeof(info)};
	PROCESS_INFORMATION proc_info = {0};

	OpenProcessToken(processHandle, TOKEN_ALL_ACCESS, &token_handle);
	DuplicateTokenEx(token_handle, TOKEN_ALL_ACCESS, NULL, SecurityImpersonation, TokenPrimary, &dup_token_handle);
	if (!CreateProcessAsUser(dup_token_handle, NULL, av[1], NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &info, &proc_info))
	{
		printf("%s() failed, error: %ld\n", "CreateProcessAsUser", GetLastError());
		return (NULL);
	}
	WaitForSingleObject(proc_info.hProcess, INFINITE);
	CloseHandle(proc_info.hProcess);
	CloseHandle(proc_info.hThread);
	CloseHandle(g_service_stop_event);
	return (NULL);
}
