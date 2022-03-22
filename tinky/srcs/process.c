#include "tinky.h"

DWORD		find_process_id(const char *process_name)
{
	HANDLE process_snap;
	PROCESSENTRY32 process_entry;
	DWORD result = 0;

	// Take a snapshot of all processes in the system.
	process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == process_snap)
		return(FALSE);
	process_entry.dwSize = sizeof(PROCESSENTRY32);
	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(process_snap, &process_entry))
	{
		printf("%s() failed, error: %ld\n", "Process32First", GetLastError());
		CloseHandle(process_snap);
		return (0);
	}
	do
	{
		if (0 == strcmp(process_name, process_entry.szExeFile))
		{
			result = process_entry.th32ProcessID;
			break;
		}
	} while (Process32Next(process_snap, &process_entry));
	CloseHandle(process_snap);
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
