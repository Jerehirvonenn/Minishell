/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:25:41 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/15 11:09:42 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_in(t_io *io_list)
{
	int	fd;

	if (io_list->amb_exp == 1)
		return (-2);
	fd = open(io_list->value, O_RDONLY);
	if (fd == -1)
	{
		if (check_file_access(io_list->value, F_OK, 2) == -1
			|| check_file_access(io_list->value, R_OK, 1) == -1)
			return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 input");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	ft_out(t_io *io_list)
{
	int	fd;

	if (io_list->amb_exp == 1)
		return (-2);
	fd = open(io_list->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		if (check_file_access(io_list->value, F_OK, 2) == -1
			|| check_file_access(io_list->value, W_OK, 1) == -1)
			return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 output");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	ft_append(t_io *io_list)
{
	int	fd;

	if (io_list->amb_exp == 1)
		return (-2);
	fd = open(io_list->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		if (check_file_access(io_list->value, F_OK, 2) == -1
			|| check_file_access(io_list->value, W_OK, 1) == -1)
			return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 append");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	ft_heredoc(t_io *io_list)
{
	if (io_list->heredoc_fd == -1)
		return (0);
	if (dup2(io_list->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 heredoc");
		close_fd(&io_list->heredoc_fd);
		return (-1);
	}
	close_fd(&io_list->heredoc_fd);
	return (0);
}

int	redirection(t_ast *node)
{
	t_io	*current_io;
	int		status;

	current_io = node->io_list;
	status = 0;
	while (current_io)
	{
		if (current_io->type == T_IN)
			status = ft_in(current_io);
		else if (current_io->type == T_OUT)
			status = ft_out(current_io);
		else if (current_io->type == T_APPEND)
			status = ft_append(current_io);
		else if (current_io->type == T_HEREDOC)
			status = ft_heredoc(current_io);
		if (status < 0)
		{
			if (status == -2)
				error_msg("minishell: ", node->io_list->value,
					": ambiguous redirect\n");
			return (-1);
		}
		current_io = current_io->next;
	}
	return (0);
}
