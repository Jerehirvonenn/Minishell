#include "../includes/minishell.h"

/**
 * init_envp - Initializes the shell's environment variable array.
 * @ms: A pointer to the shell structure containing the environment array.
 *
 * This function copies the system's environment variables 
 * (accessible via `environ`) into the shell's own `my_envp` array.
 * It calculates the size of the environment, allocates memory for the copy,
 * and duplicates each variable using `ft_strdup`. If any memory allocation
 * fails, the function cleans up and exits.
 *
 * The `my_envp` array is null-terminated for proper use with execve
 * and other functions expecting environment variables.
 */
void	init_envp(t_ms *ms)
{
	extern char	**environ;
	int			i;

	while (environ[ms->envp_size] != NULL)
		ms->envp_size++;
	ms->my_envp = (char **)malloc(sizeof (char *) * (ms->envp_size + 1));
	if (!ms->my_envp)
	{
		clean_ms(ms);
		printf("minishell: cannot allocate memory\n");
		return ;//exit??
	}
	i = 0;
	while (i < ms->envp_size)
	{
		ms->my_envp[i] = ft_strdup(environ[i]);
		if (!ms->my_envp[i]) 
		{
			while (i > 0)
				free(ms->my_envp[--i]);
			free(ms->my_envp);
			return ;//return or exit?
		}
		i++;
	}
	ms->my_envp[ms->envp_size] = NULL;
}

/**
 * envp_exists - Searches for a specified environment variable by name.
 * @name: The name of the environment variable (e.g., "PATH").
 * @ms: A pointer to the shell structure containing environment variables.
 *
 * This function iterates through the shell's environment variables to find
 * the specified one. If found, the value after the '=' sign is returned.
 *
 * Returns:
 *   The value of the environment variable if found, or NULL if not found.
 */
//check in builtins
char	*envp_exists(char *name, t_ms *ms)
{
	int		i;
	int		len;
	char	*temp;

	i = 0;
	temp = ft_strjoin(name, "=");
	if (!temp)
		return (NULL);
	len = ft_strlen(temp);
	while (ms->my_envp[i++])
	{
		if (ms->my_envp[i] == NULL)
		{
			free(temp);
			return (NULL);
		}
		if (ft_strnstr(ms->my_envp[i], temp, len))
		{
			free(temp);
			return (ms->my_envp[i] + len);
		}
	}
	free(temp);
	return (NULL);
}
