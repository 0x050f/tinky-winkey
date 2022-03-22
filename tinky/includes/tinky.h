#ifndef TINKY_H
# define TINKY_H

# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <tlhelp32.h>

# define SERVICE_NAME "tinky"
# define EXEC_NAME "winkey.exe"

extern SERVICE_STATUS			g_service_status;
extern SERVICE_STATUS_HANDLE	g_status_handle;
extern HANDLE					g_service_stop_event;

/* process.c */
DWORD							find_process_id(const char *process_name);
void							kill_process_id(DWORD pid);

/* service_handler.c */
void							report_service_status(DWORD dw_status);
DWORD WINAPI					service_control_handler(DWORD dw_ctrl);
LPSERVICE_MAIN_FUNCTIONA		service_main(DWORD ac, LPSTR *av);

/* options.c */
int								delete(SC_HANDLE scm);
int								install(SC_HANDLE scm, char *bin_path);
int								stop(SC_HANDLE scm);
int								start(SC_HANDLE scm, char *bin_path);

#endif
