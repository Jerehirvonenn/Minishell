/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:22:26 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/14 10:35:30 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Function to check if the file exists
static int	check_file_existence(char *cmd_path, t_ast *node, t_ms *ms)
{
	if (access(cmd_path, F_OK))
	{
		error_msg("minishell: ", node->exp_value[0],
			": No such file or directory\n");
		free(cmd_path);
		ms->exit_code = 127;
		return (-1);
	}
	return (0);
}

// Function to check execute permissions
static int	check_execute_permission(char *cmd_path, t_ast *node, t_ms *ms)
{
	if (access(cmd_path, X_OK))
	{
		error_msg("minishell: ", node->exp_value[0], ": Permission denied\n");
		free(cmd_path);
		ms->exit_code = 126;
		return (-1);
	}
	return (0);
}

// Function to handle the "is a directory" error
static int	check_is_directory_error(char *cmd_path, t_ast *node, t_ms *ms)
{
	if (access(cmd_path, F_OK) == 0)
	{
		error_msg("minishell: ", node->exp_value[0], ": Is a directory\n");
		free(cmd_path);
		ms->exit_code = 126;
		return (-1);
	}
	return (0);
}

static int	ret_failure(char *cmd_path, t_ast *node, t_ms *ms)
{
	if (check_execute_permission(cmd_path, node, ms)
		|| (check_is_directory_error(cmd_path, node, ms)))
		return (-1);
	ms->exit_code = 1;
	free(cmd_path);
	return (-1);
}

int	exec_bin(t_ms *ms, t_ast *node)
{
	int		ret;
	char	*cmd_path;

	ret = 0;
	cmd_path = build_executable(node, ms);
	if (cmd_path)
	{
		if (check_file_existence(cmd_path, node, ms))
			return (-1);
		ret = execve(cmd_path, node->exp_value, ms->my_envp);
		if (ret == -1)
			return (ret_failure(cmd_path, node, ms));
	}
	else
	{
		error_msg(NULL, node->exp_value[0], ": command not found\n");
		clean_and_exit(ms, 127);
	}
	free(cmd_path);
	return (ret);
}
