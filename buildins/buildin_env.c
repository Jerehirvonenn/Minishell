#include"minishell.h"

void	buildin_env(t_shell *ms, int i, int j)
{
	while (ms->my_envp[i] != NULL)
	{
		j = 0;
		while (ms->my_envp[i][j] != '=' && ms->my_envp[i][j] != '\0')
			j++;
		if (ms->my_envp[i][j] == '=')
			ft_putendl_fd(ms->my_envp[i], 1);
		i++;
	}
	ms->excode = 0;
}

/*int	main(int ac, char **av, char **envp)
{
	t_shell ms;

	ms.envp_size = 0;
	init_envp(&ms, envp);
	ms.excode = -1;  // Set a non-zero initial value for testing
	mini_env(&ms, 0, 0);
}*/
