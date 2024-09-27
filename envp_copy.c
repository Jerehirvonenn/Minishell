#include "includes/minishell.h"

/**
 * init_envp - Initializes the shell's environment variable array.
 * @ms: A pointer to the shell structure containing the environment array.
 *
 * This function copies the system's environment variables (accessible via `environ`)
 * into the shell's own `my_envp` array. It calculates the size of the environment,
 * allocates memory for the copy, and duplicates each variable using `ft_strdup`.
 * If any memory allocation fails, the function cleans up and exits.
 *
 * The `my_envp` array is null-terminated for proper use with execve and other functions
 * expecting environment variables.
 */
void	init_envp(t_shell *ms)
{
	extern char	**environ;
	int			i;

	while (environ[ms->envp_size] != NULL)
		ms->envp_size++;
	ms->my_envp = (char **)malloc(sizeof (char *) * (ms->envp_size + 1));
	if (!ms->my_envp)
		return ;
	i = 0;
	while (i < ms->envp_size)
	{
		ms->my_envp[i] = ft_strdup(environ[i]);
		if (!ms->my_envp[i]) // Check for strdup failure
		{
			while (i > 0) // Free already allocated memory
				free(ms->my_envp[--i]);
			free(ms->my_envp);
			return ;
		}
		i++;
	}
	ms->my_envp[ms->envp_size] = NULL;
}
