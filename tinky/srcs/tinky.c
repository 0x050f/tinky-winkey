#include "tinky.h"

int			main(int ac, char **av)
{
	(void)ac;
	printf("Hello world !\n");
	SC_HANDLE scm, scs;
	scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!scm)
	{
		printf("%s: %s() failed, error: %ld\n", av[0], "OpenSCManager", GetLastError());
		return (1);
	}
	scs = OpenService(scm, "tinky", DELETE);
	if (scs)
	{
		if (!DeleteService(scs))
			printf("%s: %s() failed, error: %ld\n", av[0], "DeleteService", GetLastError());
		if (!CloseServiceHandle(scs))
			printf("%s: %s() failed, error: %ld\n", av[0], "CloseServiceHandle", GetLastError());
	}
	scs = CreateService(scm, "tinky", "tinky", SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, av[0], NULL, NULL, NULL, NULL, NULL);
	if (!scs)
	{
		printf("%s: %s() failed, error: %ld\n", av[0], "CreateService", GetLastError());
		return (1);
	}
	printf("service !\n");
	return (0);
}
