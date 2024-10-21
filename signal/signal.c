#include "../includes/minishell.h"

void	ft_sigint_parent(int num)
{
	(void)num;

	printf("Ctrl+C caught\n");
	/*ft_putstr_fd("\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();*/
}

void	signal_handler()
{
	signal(SIGINT, ft_sigint_parent);
	signal(SIGQUIT, SIG_IGN);
}
