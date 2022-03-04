#include "tinky.h"

int			delete(SC_HANDLE scm)
{
	SC_HANDLE scs = OpenService(scm, SERVICE_NAME, DELETE);
	if (scs)
	{
		if (!DeleteService(scs))
			printf("%s() failed, error: %ld\n", "DeleteService", GetLastError());
		if (!CloseServiceHandle(scs))
			printf("%s() failed, error: %ld\n", "CloseServiceHandle", GetLastError());
	}
	return (0);
}

int			install(SC_HANDLE scm, char *bin_path)
{
	delete(scm);
	SC_HANDLE scs = CreateService(scm, SERVICE_NAME, SERVICE_NAME, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, bin_path, NULL, NULL, NULL, NULL, NULL);
	if (!scs)
	{
		printf("%s() failed, error: %ld\n", "CreateService", GetLastError());
		return (1);
	}
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
	if (!strcmp(av[1], "install"))
		return (install(scm, av[0]));
	else if (!strcmp(av[1], "start"))
	{
	}
	else if (!strcmp(av[1], "stop"))
	{
	}
	else if (!strcmp(av[1], "delete"))
		return (delete(scm));
	return (0);
}
