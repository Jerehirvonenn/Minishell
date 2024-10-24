#include "../includes/minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	clean_ms(t_ms *ms)
{
	free_array(ms->my_envp);
	free(ms->pwd);
	free(ms->old_pwd);
}
