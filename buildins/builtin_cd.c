/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 10:10:44 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/14 17:53:51 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_the_path(char *str, t_ms *ms)
{
	char	*p;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(str);
	while (ms->my_envp[i])
	{
		if (!ft_strncmp(ms->my_envp[i], str, len))
		{
			p = ft_substr(ms->my_envp[i], len, ft_strlen(ms->my_envp[i]) - len);
			if (!p)
			{
				malloc_parent_failure(ms);
				return (NULL);
			}
			return (p);
		}
		i++;
	}
	return (NULL);
}

static void	find_path_fail(t_ms *ms)
{
	ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	ms->exit_code = 1;
	ms->pwd = NULL;
}

static int	change_dir_path(t_ms *ms, char *str)
{
	char	*tmp;
	int		ret;

	free(ms->old_pwd);
	ms->old_pwd = ms->pwd;
	tmp = find_the_path(str, ms);
	if (!tmp)
	{
		find_path_fail(ms);
		return (-2);
	}
	ret = chdir(tmp);
	free(tmp);
	if (ret != 0)
	{
		ms->pwd = NULL;
		ms->exit_code = 1;
		return (ret);
	}
	ms->pwd = getcwd(NULL, 0);
	if (!ms->pwd)
		malloc_parent_failure(ms);
	update_pwds_envp(ms);
	return (ret);
}

static void	update_pwds(t_ms *ms, char *cmd)
{
	char	*tmp;

	tmp = ft_strdup(ms->pwd);
	if (!tmp)
		malloc_parent_failure(ms);
	free(ms->old_pwd);
	ms->old_pwd = tmp;
	free(ms->pwd);
	ms->pwd = getcwd(NULL, 0);
	if (!ms->pwd)
	{
		handle_pwd_errors(ms, cmd);
		return ;
	}
	update_pwds_envp(ms);
}

int	builtin_cd(t_ms *ms, t_ast *ast, char *cmd)
{
	int	ret;

	if (check_cd_args(ast, ms) == -1)
		return (-1);
	if (check_filename_length(cmd, ms))
		return (-1);
	if (ast->no_exp == 1)
		return (0);
	if (!cmd || (ft_strncmp(cmd, "~", 2) == 0) || !*cmd)
		ret = change_dir_path(ms, "HOME=");
	else if (ft_strncmp(cmd, "-", 2) == 0)
	{
		if (!ms->old_pwd)
			printf("%s\n", ms->pwd);
		else
		{
			ret = chdir(ms->old_pwd);
			if (ret != -1)
				printf("%s\n", ms->old_pwd);
		}
	}
	else if (ft_strncmp(cmd, "..", 3) == 0)
		ret = chdir(cmd);
	else
		ret = chdir(cmd);
	if (ret == -1)
		return (handle_cd_error(cmd, ms));
	if (cmd && (ft_strncmp(cmd, "~", 2) != 0) && *cmd)
		update_pwds(ms, cmd);
	return (ret);
}
