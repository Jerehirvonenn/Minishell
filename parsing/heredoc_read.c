/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:54:14 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/14 10:50:26 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	remove_delim_quotes(char *delim)
{
	char	*temp;
	char	quote;

	quote = 0;
	temp = delim;
	while (*temp)
	{
		if (quote == *temp)
			quote = 0;
		else if (quote == 0 && (*temp == '\'' || *temp == '\"'))
			quote = *temp;
		else
		{
			*delim = *temp;
			delim++;
		}
		temp++;
	}
	*delim = 0;
}

static int	ft_delim_expansion(char *delim)
{
	int	i;
	int	ret;

	i = -1;
	ret = 0;
	while (delim[++i])
		if (delim[i] == '\'' || delim[i] == '"')
			ret = 1;
	if (ret == 1)
	{
		remove_delim_quotes(delim);
		return (1);
	}
	else
		return (0);
}

static void	handle_heredoc_exit(char *line, char *delim, t_ms *ms)
{
	if (!line)
		error_msg("minishell: warning: "
			"here-document delimited by end of-file (wanted `",
			delim, "\')\n");
	if (ms_signal)
	{
		ms->exit_code = 130;
		ms->stop = 1;
	}
	free(line);
}

static void	handle_expansion(char *line, t_ms *ms, int fd_write)
{
	ms->tmp1 = line;
	line = expand_argument(line, ms);
	if (heredoc_write(ms, line, fd_write))
	{
		free(ms->tmp1);
		ms->tmp1 = NULL;
	}
	else
	{
		free(ms->tmp1);
		ms->tmp1 = NULL;
	}
}

int	ft_heredoc_getline(char *delim, int fd_write, t_ms *ms)
{
	char	*line;
	int		expand;

	signal_handler_heredoc();
	expand = ft_delim_expansion(delim);
	line = NULL;
	while (1)
	{
		line = readline(">");
		if (!line || ms_signal || !ft_strcmp(line, delim))
			break ;
		ms_signal = 0;
		if (expand == 0)
			handle_expansion(line, ms, fd_write);
		else if (heredoc_write(ms, line, fd_write))
			break ;
		free(line);
		line = NULL;
	}
	handle_heredoc_exit(line, delim, ms);
	signal_handler_parent();
	return (0);
}
