#ifndef EXECUTION_H
# define EXECUTION_H

enum
{
	CMD_NOPIPE,
	CMD_FIRST,
	CMD_MIDDLE,
	CMD_LAST
};

typedef struct s_shell
{
	char			**my_envp;
	int			exit_code;
	int			envp_size;
	int			pipe[2];
	int			std[2];
	char			*pwd;
	t_ast			*ast;
}	t_shell;

#endif
