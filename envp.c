#include "minishell.h"

//in this function i make a copy of the envp for future changes
// + i have access to envar vars using this $HOME $PATH

//use external char **environ inside the function 
void	init_envp(t_shell *ms)
{
	int	i;
	extern char	**environ;

	while (environ[ms->envp_size] != NULL)
		ms->envp_size++;
	ms->my_envp = (char **)malloc(sizeof (char *) * (ms->envp_size + 1));
	if (!ms->my_envp)
		return ;
	i = 0;
	while (i < ms->envp_size)
	{
		ms->my_envp[i] = ft_strdup(environ[i]);
		if (!ms->my_envp[i])  // Check for strdup failure
		{
			while (i > 0)  // Free already allocated memory
				free(ms->my_envp[--i]);
			free(ms->my_envp);
			return ;  // Handle strdup failure
		}
		i++;
	}
	ms->my_envp[ms->envp_size] = NULL;
}

void	init_shell(t_shell *ms)
{
	ft_bzero(ms, sizeof(*ms));
	ms->envp_size = 0;
	init_envp(ms);
	ms->pwd = getcwd(NULL, 0);

	ms->excode = 1;
}
