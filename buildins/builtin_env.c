/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:14:06 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/14 11:45:05 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../includes/minishell.h"

void	update_pwds_envp(t_ms *ms)
{
	char	*updated_pwd;
	char	*updated_oldpwd;
	char	*path_pwd;
	char	*path_oldpwd;

	updated_pwd = ft_strjoin("PWD=", ms->pwd);
	if (!updated_pwd)
		malloc_parent_failure(ms);
	updated_oldpwd = ft_strjoin("OLDPWD=", ms->old_pwd);
	if (!updated_oldpwd)
	{
		free(updated_pwd);
		malloc_parent_failure(ms);
	}
	path_pwd = find_the_path("PWD=", ms);
	path_oldpwd = find_the_path("OLDPWD=", ms);
	if (ms->pwd && path_pwd)
		envp_update(ms, updated_pwd);
	if (ms->old_pwd && path_oldpwd)
		envp_update(ms, updated_oldpwd);
	free(path_pwd);
	free(path_oldpwd);
	free(updated_pwd);
	free(updated_oldpwd);
}

void	builtin_env(t_ms *ms)
{
	int	i;

	i = 0;
	while (ms->my_envp[i] != NULL)
	{
		if (strchr(ms->my_envp[i], '='))
			ft_putendl_fd(ms->my_envp[i], 1);
		i++;
	}
}
