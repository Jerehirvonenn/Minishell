#include "minishell.h"

void	execute_buildin(t_shell *ms, char **av);
bool	special_buildin(int ac, char **av);

int	main(int ac, char **av)
{
	t_shell ms;
	char	*input;

	init_shell(&ms);
	//this is for cases of special buildins
	/*if (special_buildin(ac, av))
		execute_buildin(&ms, av);*/
	execute_buildin(&ms, av);
	/*while (true)
	{
		input = readline("\033[33mminishell\033[0m\033[35m$\033[0m ");
		if (input)
            		add_history(input); // Add input to history
        }*/
}
