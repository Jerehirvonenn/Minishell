# Name of the executable
NAME = minishell

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Directories
LIBFT_DIR = ./Libft
INCLUDES_DIR = ./includes
PARSING_DIR = ./parsing
TOKEN_DIR = ./token

# Library files
LIBFT = $(LIBFT_DIR)/libft.a

# Source files
PARSING_SRCS = $(PARSING_DIR)/parsing.c
TOKEN_SRCS = $(TOKEN_DIR)/print_ast.c $(TOKEN_DIR)/token.c $(TOKEN_DIR)/token_create.c $(TOKEN_DIR)/main.c

# Object files
PARSING_OBJS = $(PARSING_SRCS:.c=.o)
TOKEN_OBJS = $(TOKEN_SRCS:.c=.o)

# All object files
OBJS = $(PARSING_OBJS) $(TOKEN_OBJS)

# Includes and headers
INCLUDES = -I$(INCLUDES_DIR) -I$(LIBFT_DIR)

# Rules

# Default target
all: $(LIBFT) $(NAME)

# Build minishell executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build the libft library
$(LIBFT):
	make -C $(LIBFT_DIR)

# Clean object files
clean:
	rm -f $(PARSING_OBJS) $(TOKEN_OBJS)
	make -C $(LIBFT_DIR) clean

# Clean object files and the executable
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re

