#ifndef PARSING_H
# define PARSING_H

#include <stdio.h> //move to main h file
#include <stdlib.h> //move to main h file

typedef enum e_type
{
	T_CMND_AST,
	T_PIPE_AST,
}	t_type;

typedef enum e_io_type
{
	T_IN,
	T_OUT,
	T_HEREDOC,
	T_APPEND,
}	t_io_type;

typedef struct s_io
{
	t_io_type	type;
	char		*value;
	struct s_io	*prev;
	struct s_io	*next;
}	t_io;

typedef struct s_ast
{
	t_token_type		type;
	char		*value;
	char		**exp_value;
	pid_t		pid;//change
	t_io		*io_list;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

t_ast	*parsing_ast(t_token *tokens);

#endif
