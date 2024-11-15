/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:07:32 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/15 09:26:57 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_empty_heredoc(t_ms *ms, t_io *io, char *delim)
{
	char	*line;

	signal_handler_heredoc(ms);
	remove_delim_quotes(delim);
	while (1)
	{
		line = readline(">");
		if (!line || g_signal || !ft_strcmp(line, delim))
			break ;
		free(line);
		line = NULL;
	}
	if (!line)
		error_msg("minishell: warning: here-document delimited "
			"by end of-file (wanted `", delim, "\')\n");
	if (g_signal)
	{
		ms->exit_code = 130;
		ms->stop = 1;
	}
	free(line);
	io->heredoc_fd = -1;
	if (signal_handler_parent())
		hdoc_signal_failure(ms, -1);
}

int	ft_heredoc(t_ms *ms, t_io *io)
{
	int	fd_read;
	int	fd_write;

	if (ft_createfile(&fd_write, &fd_read))
	{
		ms->stop = 1;
		return (1);
	}
	io->heredoc_fd = fd_read;
	ft_heredoc_getline(io->value, fd_write, ms);
	return (0);
}

int	ft_heredoc_used(t_io *io)
{
	io = io->next;
	while (io)
	{
		if (io->type == T_HEREDOC || io->type == T_IN)
			return (1);
		io = io->next;
	}
	return (0);
}

int	ast_heredoc(t_ast *tree, t_ms *ms)
{
	t_io	*io_temp;

	if (ms->stop || !tree)
		return (1);
	io_temp = tree->io_list;
	while (!ms->stop && tree->type == T_CMND && io_temp)
	{
		if (io_temp->type == T_HEREDOC)
		{
			if (ft_heredoc_used(io_temp))
				ft_empty_heredoc(ms, io_temp, io_temp->value);
			else
				ft_heredoc(ms, io_temp);
		}
		io_temp = io_temp->next;
	}
	if (tree->left && !ms->stop)
		ast_heredoc(tree->left, ms);
	if (tree->right && !ms->stop)
		ast_heredoc(tree->right, ms);
	return (0);
}
