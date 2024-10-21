#ifndef MINISHELL_H
# define MINISHELL_H

#include "../Libft/libft.h"
#include "token.h"
#include "parsing.h"
#include "execution.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct s_ms
{
	char    	**my_envp;	// Environment variables
	int     	exit_code;	// Exit code for the shell
	int     	envp_size;	// Size of the environment variables
	int		ms_stop;	//indicate the minishell should stop and start new round
	int		ms_quit;	//indicate minishell should quit
	char    	*pwd;		// Current working directory
	char		*old_pwd;
	t_token		*tokens		//linked list of tokens
	t_ast   	*ast;		// Abstract syntax tree for the command(s)
}	t_ms;

void	signal_handler();

#endif
