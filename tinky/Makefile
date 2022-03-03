#define macros
NAME = tinky.exe
SRC = srcs
BIN = bin
INT = objs
DIR_INCLUDES = includes

CFLAGS = /Wall /WX

all: create_dirs make_exe

make_exe: $(NAME)

{$(SRC)}.c{$(INT)}.obj ::
		@echo Compiling...
		cl $(CFLAGS) /Fo$(INT)\ /I$(DIR_INCLUDES) $<

# link the files into an executable
$(NAME): $(INT)\*.obj
		@echo Linking $(NAME)...
		link /out:$(NAME) $(INT)\*.obj

# create output directories
create_dirs:
		@if not exist $(INT) mkdir $(INT)

# delete output directories
clean:
		@if exist $(INT) rmdir /s /q $(INT)

fclean: clean
		@if exist $(NAME) del /f /s /q $(NAME) 1>nul
