#ifndef PARSING_H
# define PARSING_H

typedef enum e_type
{
	T_PIPE,
	T_CMND,
}	t_type;

typedef enum e_io_type
{
	T_IN,
	T_OUT,
	T_HERE_DOC,
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
	t_type		type;
	char		*value;
	char		**exp_value;
	t_io		*io_list;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

#endif
