/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:58:39 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/13 18:00:29 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_middle_command(t_ast *node, t_ms *ms, int *pipefd, int *wr_p)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		handle_fork_failure(ms);
		return ;
	}
	else if (node->pid == 0)
	{
		if ((dup2(pipefd[0], 0) == -1) || (dup2(wr_p[1], 1) == -1))
			handle_dup2_failure(ms);
		close_array_fds(ms);
		if (redirection(node) == -1)
		{
			ft_free_ast(ms->ast);
			clean_ms(ms);
			exit(EXIT_FAILURE);
		}
		child_process(ms, node);
		exit(EXIT_SUCCESS);
	}
	close_and_change_array(ms, &pipefd[0]);
	close_and_change_array(ms, &wr_p[1]);
}

void	execute_last_command(t_ast *node, t_ms *ms, int *pipefd)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		handle_fork_failure(ms);
		return ;
	}
	else if (node->pid == 0)
	{
		if (dup2(pipefd[0], 0) == -1)
			handle_dup2_failure(ms);
		close_array_fds(ms);
		if (redirection(node) == -1)
		{
			ft_free_ast(ms->ast);
			clean_ms(ms);
			exit(EXIT_FAILURE);
		}
		child_process(ms, node);
		exit(EXIT_SUCCESS);
	}
	close_and_change_array(ms, &pipefd[0]);
}

void	execute_first_command(t_ast *node, t_ms *ms, int *pipefd)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		handle_fork_failure(ms);
		return ;
	}
	else if (node->pid == 0)
	{
		if (dup2(pipefd[1], 1) == -1)
			handle_dup2_failure(ms);
		close_array_fds(ms);
		if (redirection(node) == -1)
		{
			ft_free_ast(ms->ast);
			clean_ms(ms);
			exit(EXIT_FAILURE);
		}
		child_process(ms, node);
		exit(EXIT_SUCCESS);
	}
	close_and_change_array(ms, &pipefd[1]);
}

void	execute_pipe(t_ast *node, t_ms *ms, int *wr_p)
{	
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe2 failure");
		ms->exit_code = 1;
		ms->stop = 1;
		return ;
	}
	add_to_array(ms, pipefd[0]);
	add_to_array(ms, pipefd[1]);
	if (node->left && node->left->type == T_PIPE)
		execute_pipe(node->left, ms, pipefd);
	else if (node->left && node->left->type == T_CMND)
		execute_first_command(node->left, ms, pipefd);
	if (node->right && node->right->type == T_CMND && ms->stop == 0)
		execute_middle_command(node->right, ms, pipefd, wr_p);
	close_multiple_fds(pipefd);
}

void	execute_first_pipe(t_ast *node, t_ms *ms)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe1 failure");
		ms->exit_code = 1;
		ms->stop = 1;
		return ;
	}
	add_to_array(ms, pipefd[0]);
	add_to_array(ms, pipefd[1]);
	if (node->left && node->left->type == T_PIPE)
		execute_pipe(node->left, ms, pipefd);
	else if (node->left && node->left->type == T_CMND)
		execute_first_command(node->left, ms, pipefd);
	if (node->right && node->right->type == T_CMND && ms->stop == 0)
		execute_last_command(node->right, ms, pipefd);
	close_multiple_fds(pipefd);
}
