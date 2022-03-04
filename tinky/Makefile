NAME = svc.exe

DIR_SRCS = srcs
DIR_OBJS = objs
DIR_INCLUDES = includes

CFLAGS = /Wall /WX

all: create_dirs make_exe

re: fclean all

make_exe: $(NAME)

{$(DIR_SRCS)}.c{$(DIR_OBJS)}.obj :
		@echo Compiling...
		cl $(CFLAGS) /c /Fo$(DIR_OBJS)\ /I$(DIR_INCLUDES) $<

# link the files into an executable
$(NAME): $(DIR_OBJS)\*.obj
		@echo Linking $(NAME)...
		link /out:$(NAME) $(DIR_OBJS)\*.obj advapi32.lib

# create output directories
create_dirs:
		@if not exist $(DIR_OBJS) mkdir $(DIR_OBJS)

# delete output directories
clean:
		@if exist $(DIR_OBJS) rmdir /s /q $(DIR_OBJS)

fclean: clean
		@if exist $(NAME) del /f /s /q $(NAME) 1>nul
