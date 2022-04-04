NAME = winkey.exe

DIR_SRCS = srcs
DIR_OBJS = objs
DIR_INCLUDES = includes

SRCS		=	$(DIR_SRCS)\winkey.c \
				$(DIR_SRCS)\hook.c \
				$(DIR_SRCS)\log.c

OBJS = $(SRCS:.c=.obj)
OBJS = $(OBJS:srcs=objs)

CFLAGS = /Wall /WX

TEST_PATH = C:\Users\User\Documents

all: create_dirs make_exe

re: fclean all

test: all
		copy $(NAME) $(TEST_PATH)

make_exe: $(NAME)

{$(DIR_SRCS)}.c{$(DIR_OBJS)}.obj :
		@echo Compiling...
		cl /nologo $(CFLAGS) /c /Fo$(DIR_OBJS)\ /I$(DIR_INCLUDES)\ $<

# link the files into an executable
$(NAME): $(OBJS)
		@echo Linking $(NAME)...
		link /NOLOGO /out:$(NAME) $(OBJS) user32.lib
		@echo Executable $(NAME) created.

# create output directories
create_dirs:
		@if not exist $(DIR_OBJS) mkdir $(DIR_OBJS)

# delete output directories
clean:
		@if exist $(DIR_OBJS) rmdir /s /q $(DIR_OBJS)

fclean: clean
		@if exist $(NAME) del /f /s /q $(NAME) 1>nul
