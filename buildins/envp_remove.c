/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_remove.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:13:04 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/14 11:17:44 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	**allocate_new_envp(int new_size, t_ms *ms)
{
	char	**new_envp;

	new_envp = malloc((new_size + 1) * sizeof(char *));
	if (!new_envp)
		malloc_parent_failure(ms);
	return (new_envp);
}

// Copy environment variables, excluding the one to be removed
static int	cp_env(t_ms *ms, char **new_envp, char *content, int len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (ms->my_envp[j])
	{
		if (!ft_strncmp(ms->my_envp[j], content, len)
			&& (ms->my_envp[j][len] == '=' || ms->my_envp[j][len] == '\0'))
		{
			j++;
			continue ;
		}
		new_envp[i] = ft_strdup(ms->my_envp[j]);
		if (!new_envp[i])
			handle_allocation_failure(new_envp, i, ms);
		i++;
		j++;
	}
	return (i);
}

int	envp_remove(t_ms *ms, char *content)
{
	int		i;
	int		len;
	int		new_size;
	char	**new_envp;

	len = ft_strlen(content);
	new_size = ms->envp_size - 1;
	new_envp = allocate_new_envp(new_size, ms);
	i = cp_env(ms, new_envp, content, len);
	new_envp[i] = NULL;
	free_array(ms->my_envp);
	ms->my_envp = new_envp;
	ms->envp_size = new_size;
	return (1);
}
