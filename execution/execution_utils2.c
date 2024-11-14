/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:52:52 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/14 10:24:46 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_fork_failure(t_ms *ms)
{
	perror("fork error");
	ms->exit_code = 1;
	ms->quit = 1;
}

// Handles dup2 failure and exits the child process
void	handle_dup2_failure(t_ms *ms)
{
	perror("dup2 error");
	close_array_fds(ms);
	ms->exit_code = 1;
	exit(EXIT_FAILURE);
}

// Function to check if the file exists
int	check_file_existence(char *cmd_path, t_ast *node, t_ms *ms)
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

void	clean_and_exit(t_ms *ms, int exit_code)
{
	ft_free_ast(ms->ast);
	clean_ms(ms);
	exit(exit_code);
}
