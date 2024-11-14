/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:59:01 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/14 11:14:07 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	error_check(char *str)
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
	if (str[i] != '\0')
		return (1);
	return (0);
}

char	*name_exists(const char *arg, t_ms *ms)
{
	int		i;
	int		len;

	len = 0;
	while (arg[len] != '\0' && arg[len] != '=')
		len++;
	i = 0;
	while (ms->my_envp[i])
	{
		if (ft_strncmp(ms->my_envp[i], arg, len) == 0
			&& (ms->my_envp[i][len] == '\0' || ms->my_envp[i][len] == '='))
			return (ms->my_envp[i]);
		i++;
	}
	return (NULL);
}

void	builtin_unset(t_ms *ms, char **cmd)
{
	int	i;

	i = 1;
	while (cmd[i] != NULL)
	{
		if (!error_check(cmd[i]))
		{
			if (name_exists(cmd[i], ms) != NULL)
			{
				if (envp_remove(ms, cmd[i]) == -1)
					return ;
			}
		}
		else
		{
			printf("unset: %s: not a valid identifier\n", cmd[i]);
			ms->exit_code = 1;
		}
		i++;
	}
}
