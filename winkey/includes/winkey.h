#ifndef WINKEY_H
# define WINKEY_H

# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <conio.h>
# include <winuser.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>

const char *key_code_table[] =
{
	[VK_BACK]				= "\\b",
	[VK_TAB]				= "\\t",
	[VK_RETURN]				= "\\n",
	[VK_SPACE]				= " ",
	[VK_LEFT]				= "[<]",
	[VK_UP]					= "[^]",
	[VK_RIGHT]				= "[>]",
	[VK_DOWN]				= "[v]",
	[VK_OEM_1]				= ";",
	[VK_OEM_PLUS]			= "=",
	[VK_OEM_COMMA]			= ",",
	[VK_OEM_MINUS]			= "-",
	[VK_OEM_PERIOD]			= ".",
	[VK_OEM_2]				= "/",
	[VK_OEM_3]				= "`",
	[VK_OEM_4]				= "[",
	[VK_OEM_5]				= "\\",
	[VK_OEM_6]				= "]",
	[VK_OEM_7]				= "'"
};

// This is for QWERTY Layout
const char special_char_table[] = 
{
	['0']						= ')',
	['1']						= '!',
	['2']						= '@',
	['3']						= '#',
	['4']						= '$',
	['5']						= '%',
	['6']						= '^',
	['7']						= '&',
	['8']						= '*',
	['9']						= '(',
	['`']						= '~',
	['[']						= '{',
	[']']						= '}',
	['\\']						= '|',
	[';']						= ':',
	['\'']						= '"',
	[',']						= '<',
	['.']						= '>',
	['/']						= '?',
	['-']						= '_',
	['=']						= '+'
};

#endif
