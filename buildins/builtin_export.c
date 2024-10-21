#include "../includes/minishell.h"

char	*name_exists(char *arg, t_ms *ms)
{
	int	i;
	int	len;
	char	*name;

	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
		i++;
	name = ft_substr(arg, 0, i);
	if (!name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (ms->my_envp[i])
	{
		if (ft_strncmp(ms->my_envp[i], name, len) == 0
			&& (ms->my_envp[i][len] == '\0' || ms->my_envp[i][len] == '='))
		{
			free(name);
			return (ms->my_envp[i] + len);
		}
		i++;
	}
	free(name);
	return (NULL);
}

int	content_check(char *arg, t_ms *ms)
{
	int		i;
	char	*content;

	i = 0;
	content = name_exists(arg, ms);
	while (arg[i] != '=' && arg[i] != '\0')
		i++;
	if (arg[i] == '=' && arg[i + 1] == '\0' && *content == '\0')
		return (1);
	else if (arg[i] == '=' && arg[i + 1] != '\0')
		return (1);
	return (0);
}

int	error_check(char *str)
{
	int	i;

	i = 0;
	if (ft_isdigit(str[i]) || str[i] == '=')
		return (1);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

void	envp_update(t_ms *ms, char *content)
{
	int		i;
	int		size;

	i = 0;
	size = 0;
	while (content[size] != '=')
		size++;
	while (i < ms->envp_size)
	{
		if (!ft_strncmp(ms->my_envp[i], content, size))
		{
			free(ms->my_envp[i]); // Free old value to prevent memory leaks
			ms->my_envp[i] = ft_strdup(content); // Copy new content
			if (!ms->my_envp[i])
				perror("envp update error");
			return;
		}
		i++;
	}
}

void	envp_add(t_ms *ms, char *content)
{
	char	**new_envp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	ms->envp_size += 1;
	new_envp = malloc((ms->envp_size + 1) * sizeof(char *));
	while (i < ms->envp_size)
	{
		if (!ft_strncmp(ms->my_envp[j], "_=", 2))  // This part
		{
			new_envp[i] = ft_strdup(content); // Copy new variable here
			if (!new_envp[i])
			{
				perror("new envp error");
				return ;
			}
			i++;
		}
		new_envp[i] = ft_strdup(ms->my_envp[j]); // Copy old env variables
		if (!new_envp[i])
		{
			perror("new envp error");
			return ;
		}
		i++;
		j++;
	}
	ms->my_envp = new_envp; // Update environment pointer
}


void	builtin_export(t_ms *ms, char **cmd, int i, int j)
{
	while (cmd[i] != NULL)
	{
		if (!error_check(cmd[i]))
		{
			if (name_exists(cmd[i], ms) != NULL && content_check(cmd[i], ms))
				envp_update(ms, cmd[i]);
			else if (name_exists(cmd[i], ms) == NULL)
				envp_add(ms, cmd[i]);
		}
		else
		{
			fprintf(stderr, "export: not a valid identifier\n");
			ms->exit_code = 1;//or ehatever name is
		}
		i++;
	}
}
