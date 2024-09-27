#ifndef EXECUTION_H
# define EXECUTION_H

typedef struct s_shell
{
	char			**my_envp;
	int			exit_code;
	int			envp_size;
	char			*cwd;
	t_ast			*ast;
}	t_shell;

#endif
