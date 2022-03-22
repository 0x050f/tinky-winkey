#include "tinky.h"

SERVICE_STATUS			g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE	g_StatusHandle = NULL;
HANDLE					g_ServiceStopEvent = INVALID_HANDLE_VALUE;

DWORD		find_process_id(const char *process_name)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 process_entry;
	DWORD result = 0;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnap) return(FALSE);

	process_entry.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &process_entry))
	{
		printf("%s() failed, error: %ld\n", "Process32First", GetLastError());
		CloseHandle(hProcessSnap);
		return (0);
	}

	do
	{
		if (0 == strcmp(process_name, process_entry.szExeFile))
		{
			result = process_entry.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnap, &process_entry));

	CloseHandle(hProcessSnap);
	return (result);
}

void		kill_process_id(DWORD pid)
{
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, pid);
	if (hProcess)
	{
		TerminateProcess(hProcess, 9);
		CloseHandle(hProcess);
	}
}

int			delete(SC_HANDLE scm)
{
	SC_HANDLE scs = OpenService(scm, SERVICE_NAME, DELETE);
	if (scs)
	{
		if (!DeleteService(scs))
			printf("%s() failed, error: %ld\n", "DeleteService", GetLastError());
		else
			printf("Service {%s} deleted successfully.\n", SERVICE_NAME);
		if (!CloseServiceHandle(scs))
			printf("%s() failed, error: %ld\n", "CloseServiceHandle", GetLastError());
	}
	else
		printf("Service {%s} is not installed.\n", SERVICE_NAME);
	return (0);
}

int			install(SC_HANDLE scm, char *bin_path)
{
	SC_HANDLE scs = OpenService(scm, SERVICE_NAME, SERVICE_ALL_ACCESS);
	if (scs)
	{
		printf("Service {%s} exists.\n", SERVICE_NAME);
		return (1);
	}
	scs = CreateService(scm, SERVICE_NAME, SERVICE_NAME, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, bin_path, NULL, NULL, NULL, NULL, NULL);
	if (!scs)
	{
		printf("%s() failed, error: %ld\n", "CreateService", GetLastError());
		return (1);
	}
	printf("Service {%s} installed successfully.\n", SERVICE_NAME);
	return (0);
}

int			stop(SC_HANDLE scm)
{
	SC_HANDLE scs = OpenService(scm, SERVICE_NAME, SERVICE_ALL_ACCESS);
	if (scs)
	{
		if (!ControlService(scs, SERVICE_CONTROL_STOP, &g_ServiceStatus))
		{
			printf("Service {%s} not started.\n", SERVICE_NAME);
			if (!CloseServiceHandle(scs))
				printf("%s() failed, error: %ld\n", "CloseServiceHandle", GetLastError());
			return (1);
		}
		printf("Service {%s} stopped successfully.\n", SERVICE_NAME);
		if (!CloseServiceHandle(scs))
			printf("%s() failed, error: %ld\n", "CloseServiceHandle", GetLastError());
	}
	else
		printf("Service {%s} is not installed.\n", SERVICE_NAME);
	return (0);
}

int			start(SC_HANDLE scm, char *bin_path)
{
	SC_HANDLE scs = OpenService(scm, SERVICE_NAME, SERVICE_ALL_ACCESS);
	if (scs)
	{
		const char *args[] = { bin_path };
		if (!StartService(scs, 1, args))
		{
			printf("%s() failed, error: %ld\n", "StartService", GetLastError());
			if (!CloseServiceHandle(scs))
				printf("%s() failed, error: %ld\n", "CloseServiceHandle", GetLastError());
			return (1);
		}
		printf("Service {%s} started successfully.\n", SERVICE_NAME);
		if (!CloseServiceHandle(scs))
			printf("%s() failed, error: %ld\n", "CloseServiceHandle", GetLastError());
	}
	else
		printf("Service {%s} is not installed.\n", SERVICE_NAME);
	return (0);
}

DWORD WINAPI service_control_handler(DWORD dwCtrl)
{
    switch (dwCtrl)
	{
		case SERVICE_CONTROL_STOP :
			kill_process_id(find_process_id(EXEC_NAME));
			break ;
		default:
			;
	}
	return NO_ERROR;
}

LPSERVICE_MAIN_FUNCTIONA service_main_function(DWORD ac, LPSTR *av)
{
	if (ac <= 1)
		return (NULL);
	g_StatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, service_control_handler);
	if (!g_StatusHandle)
	{
		printf("%s() failed, error: %ld\n", "RegisterServiceCtrlHandler", GetLastError());
		return (NULL);
	}
	g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_ServiceStatus.dwControlsAccepted = 0;
	g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwServiceSpecificExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;
	if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		printf("%s() failed, error: %ld\n", "SetServiceStatus", GetLastError());
		return (NULL);
	}
	g_ServiceStopEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
	if (g_ServiceStopEvent == NULL) 
    {
		printf("%s() failed, error: %ld\n", "CreateEvent", GetLastError());
		return (NULL);
	}
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PRESHUTDOWN;
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;
	if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE)
	{
		printf("%s() failed, error: %ld\n", "SetServiceStatus", GetLastError());
		return (NULL);
	}
	HANDLE duplicateTokenHandle = NULL;
	HANDLE tokenHandle = NULL;
	DWORD pid = find_process_id("winlogon.exe");
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, true, pid);
	STARTUPINFO info={sizeof(info)};
	PROCESS_INFORMATION processInfo = {0};

	OpenProcessToken(processHandle, TOKEN_ALL_ACCESS, &tokenHandle);
	DuplicateTokenEx(tokenHandle, TOKEN_ALL_ACCESS, NULL, SecurityImpersonation, TokenPrimary, &duplicateTokenHandle);
	if (!CreateProcessAsUser(duplicateTokenHandle, NULL, av[1], NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &info, &processInfo))
	{
		printf("%s() failed, error: %ld\n", "CreateProcessAsUser", GetLastError());
		return (NULL);
	}
	WaitForSingleObject(processInfo.hProcess, INFINITE);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	CloseHandle(g_ServiceStopEvent);
	return (NULL);
}

int			main(int ac, char **av)
{
	if (ac >= 2 && (strcmp(av[1], "install") && \
					strcmp(av[1], "start") && \
					strcmp(av[1], "stop") && \
					strcmp(av[1], "delete")))
	{
		printf("Usage: %s {install,start,stop,delete}\n", strrchr(av[0], '\\') + 1);
		return (1);
	}
	else if (ac <= 1)
	{
		SERVICE_TABLE_ENTRY service_table_entry[] =
		{
			{SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) service_main_function},
			{NULL, NULL}
		};
		if (!StartServiceCtrlDispatcher(service_table_entry))
		{
			printf("%s() failed, error: %ld\n", "StartServiceCtrlDispatcher", GetLastError());
			return (1);
		}
		return (0);
	}
	SC_HANDLE scm;
	scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!scm)
	{
		printf("%s() failed, error: %ld\n", "OpenSCManager", GetLastError());
		return (1);
	}
	int ret = 0;
	if (!strcmp(av[1], "install"))
		ret = install(scm, av[0]);
	else if (!strcmp(av[1], "start"))
	{
		char		path[256];

		memcpy(path, av[0], strlen(av[0]) + 1);
		memcpy(strrchr(path, '\\') + 1, EXEC_NAME, strlen(EXEC_NAME) + 1);
		ret = start(scm, path);
	}
	else if (!strcmp(av[1], "stop"))
		ret = stop(scm);
	else if (!strcmp(av[1], "delete"))
		ret = delete(scm);
	if (!CloseServiceHandle(scm))
		printf("%s() failed, error: %ld\n", "CloseServiceHandle", GetLastError());
	return (ret);
}
