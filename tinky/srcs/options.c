#include "tinky.h"

int			delete(SC_HANDLE scm)
{
	SC_HANDLE scs = OpenService(scm, SERVICE_NAME, DELETE);
	if (scs)
	{
		if (!DeleteService(scs))
		{
			printf("%s() failed, error: %ld\n", "DeleteService", GetLastError());
			return (1);
		}
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
		if (!ControlService(scs, SERVICE_CONTROL_STOP, &g_service_status))
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
			DWORD error_code = GetLastError();
			if (error_code == ERROR_SERVICE_ALREADY_RUNNING)
				printf("Service {%s} already running.\n", SERVICE_NAME);
			else
				printf("%s() failed, error: %ld\n", "StartService", error_code);
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

