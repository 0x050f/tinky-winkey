#include "tinky.h"

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

int			start(SC_HANDLE scm)
{
	SC_HANDLE scs = OpenService(scm, SERVICE_NAME, SERVICE_ALL_ACCESS);
	if (scs)
	{
		if (!StartService(scs, 0, NULL))
		{
			printf("%s() failed, error: %ld\n", "StartService", GetLastError());
			if (!CloseServiceHandle(scs))
				printf("%s() failed, error: %ld\n", "CloseServiceHandle", GetLastError());
			return (1);
		}
		printf("Service {%s} started successfully.\n", SERVICE_NAME);
	}
	else
		printf("Service {%s} is not installed.\n", SERVICE_NAME);
	return (0);
}

int			main(int ac, char **av)
{
	if (ac != 2 || (strcmp(av[1], "install") && \
					strcmp(av[1], "start") && \
					strcmp(av[1], "stop") && \
					strcmp(av[1], "delete")))
	{
		printf("Usage: %s {install,start,stop,delete}\n", strrchr(av[0], '\\') + 1);
		return (1);
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
	{
		int size = strrchr(av[0], '\\') + 1 - av[0];
		char *str = malloc(size + strlen(EXEC_NAME) + 1);
		memcpy(str, av[0], size);
		memcpy(str + size, EXEC_NAME, strlen(EXEC_NAME) + 1);
		ret = install(scm, str);
		free(str);
	}
	else if (!strcmp(av[1], "start"))
		ret = start(scm);
	else if (!strcmp(av[1], "stop"))
	{
	}
	else if (!strcmp(av[1], "delete"))
		ret = delete(scm);
	if (!CloseServiceHandle(scm))
		printf("%s() failed, error: %ld\n", "CloseServiceHandle", GetLastError());
	return (ret);
}
