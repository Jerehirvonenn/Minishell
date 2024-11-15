/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:57:04 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/15 10:34:17 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_builtin(t_ast *node, t_ms *ms)
{
	if (node->io_list)
	{
		if (redirection_parent(node) == -1)
			return ;
	}
	exec_builtin(ms, node);
}

static void	execute_command(t_ast *node, t_ms *ms)
{
	if (is_builtin(node))
		handle_builtin(node, ms);
	else
	{
		node->pid = fork();
		if (node->pid == -1)
		{
			perror("fork error");
			ms->exit_code = 1;
			return ;
		}
		else if (node->pid == 0)
		{
			if (redirection(node) == -1)
			{
				ft_free_ast(ms->ast);
				clean_ms(ms);
				exit(EXIT_FAILURE);
			}
			child_process(ms, node);
			exit(EXIT_SUCCESS);
		}
	}
}

// WIFEXITED and WEXITSTATUS are macros
static void	ft_waiting(t_ast *node, t_ms *ms)
{
	static int	status;

	if (node->pid != -1 && node->type == T_CMND)
	{
		if (node->type == T_CMND)
			waitpid(node->pid, &status, 0);
		if (WIFEXITED(status))
			ms->exit_code = WEXITSTATUS(status);
	}
	if (node->left)
		ft_waiting(node->left, ms);
	if (node->right)
		ft_waiting(node->right, ms);
}

void	execute_ast(t_ast *node, t_ms *ms)
{
	int	i;

	i = 0;
	if (signal_handler_exec() == -1)
	{
		ft_free_ast(ms->ast);
		clean_ms(ms);
		exit(1);
	}
	while (i < 1024)
		ms->fds[i++] = -1;
	if (!node)
		return ;
	if (node->type == T_PIPE)
		execute_first_pipe(node, ms);
	else if (node->type == T_CMND)
		execute_command(node, ms);
	ft_waiting(node, ms);
	if (g_signal == SIGINT)
		ms->exit_code = 130;
	if (g_signal == SIGQUIT)
		ms->exit_code = 131;
}
