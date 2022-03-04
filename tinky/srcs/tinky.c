#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

int			main(void)
{
	printf("Hello world !\n");
	SC_HANDLE svc = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!svc)
	{
		printf("ko\n");
		return (1);
	}
	printf("youhou\n");
	return (0);
}
