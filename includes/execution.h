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
	int			pipefd[2];// Array to hold pipe file descriptors
	pid_t 			pids[256];// Store PIDs of child processes
	int			tempfd;// Temporary file descriptor
	int index;     // Current index for PIDs
	char			*pwd;
	t_ast			*ast;
}	t_shell;

#endif
