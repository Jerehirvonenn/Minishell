#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../includes/minishell.h"

void	ft_sigint_parent(int num)
{
	ms_signal = num;
	ft_putstr_fd("\n", STDOUT_FILENO);
	//rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay(); // Redraw the prompt
}

void	signal_handler()
{
	signal(SIGINT, ft_sigint_parent);
	signal(SIGQUIT, SIG_IGN);
}
