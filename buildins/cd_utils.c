/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:27:10 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/15 10:24:15 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_cd_dash(t_ms *ms, int *ret)
{
	if (!ms->old_pwd)
	{
		printf("%s\n", ms->pwd);
		return (0);
	}
	*ret = chdir(ms->old_pwd);
	if (*ret != -1)
		printf("%s\n", ms->old_pwd);
	return (*ret);
}

int	check_cd_args(t_ast *ast, t_ms *ms)
{
	int	i;

	i = 0;
	while (ast->exp_value[i])
		i++;
	if (i > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		ms->exit_code = 1;
		return (-1);
	}
	return (0);
}

// Handle errors related to file/directory access
int	handle_cd_error(char *cmd, t_ms *ms)
{
	if (access(cmd, F_OK) == -1)
		error_msg("minishell: cd: ", cmd, ": No such file or directory\n");
	else if (access(cmd, X_OK) == -1)
		error_msg("minishell: cd: ", cmd, ": Permission denied\n");
	ms->exit_code = 1;
	return (-1);
}

// Check for file name length and handle errors
int	check_filename_length(char *cmd, t_ms *ms)
{
	if (!cmd)
		return (0);
	if (ft_strlen(cmd) > 255)
	{
		error_msg("minishell: cd: ", cmd, ": File name too long\n");
		ms->exit_code = 1;
		return (-1);
	}
	return (0);
}

void	handle_pwd_errors(t_ms *ms, char *cmd)
{
	if (errno == ENOENT)
	{
		ft_putstr_fd("cd: error retrieving current directory:", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		if (ft_strncmp(cmd, "..", 3) == 0)
		{
			ms->pwd = ft_strjoin(ms->old_pwd, "/..");
			if (!ms->pwd)
				malloc_parent_failure(ms);
		}
		else if (ft_strncmp(cmd, ".", 2) == 0)
		{
			ms->pwd = ft_strjoin(ms->old_pwd, "/.");
			if (!ms->pwd)
				malloc_parent_failure(ms);
		}
	}
}
