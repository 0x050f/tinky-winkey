#include "tinky.h"

SERVICE_STATUS			g_service_status = {0};
SERVICE_STATUS_HANDLE	g_status_handle = NULL;
HANDLE					g_service_stop_event = INVALID_HANDLE_VALUE;

int			main(int ac, char **av)
{
	int			ret = 0;

	if (ac >= 2 && (strcmp(av[1], "install") && \
					strcmp(av[1], "start") && \
					strcmp(av[1], "stop") && \
					strcmp(av[1], "delete")))
	{
		printf("Usage: %s {install,start,stop,delete}\n", (strchr(av[0], '\\')) ? strrchr(av[0], '\\') + 1 : av[0]);
		return (1);
	}
	else if (ac <= 1)
	{
		SERVICE_TABLE_ENTRY service_table_entry[] =
		{
			{SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) service_main},
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
		ret = GetLastError();
		if (ret == ERROR_ACCESS_DENIED)
			printf("%s must be launched with admin rights\n", (strchr(av[0], '\\')) ? strrchr(av[0], '\\') + 1 : av[0]);
		else
			printf("%s() failed, error: %ld\n", "OpenSCManager", GetLastError());
		return (1);
	}
	WCHAR		full_path[MAX_PATH];
	char		path[MAX_PATH];
	size_t		n = 0;

	GetModuleFileNameW(NULL, full_path, MAX_PATH);
	wcstombs_s(&n, path, MAX_PATH, full_path, wcslen(full_path));
	if (!strcmp(av[1], "install"))
		ret = install(scm, path);
	else if (!strcmp(av[1], "start"))
	{
		memcpy((strchr(path, '\\')) ? strrchr(path, '\\') + 1 : path, EXEC_NAME, strlen(EXEC_NAME) + 1);
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
