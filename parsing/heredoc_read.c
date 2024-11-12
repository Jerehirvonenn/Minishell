/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:54:14 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/12 16:37:24 by jhirvone         ###   ########.fr       */
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

int	ft_heredoc_getline(t_io *io, char *delim, int fd_write, t_ms *ms)
{
	char	*line;
	int		expand;

	if (!io)    //take io away
		exit(1);
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
		{
			ms->tmp1 = line;
			line = expand_argument(line, ms);
			write(fd_write, line, ft_strlen(line));
			write(fd_write, "\n", 1);//check write errors
			free(ms->tmp1);  //is this whats suppsoe to happen?
			ms->tmp1 = NULL;
		}
		else
		{
			write(fd_write, line, ft_strlen(line));
			write(fd_write, "\n", 1);
		}
		free(line);
		line = NULL;
	}
	if (!line)
		printf("minishell: warning: here-document delimited by end of-file (wanted %s)\n", delim);
	if (ms_signal)
	{
		ms->exit_code = 130;
		ms->stop = 1;
	}
	free(line);
	signal_handler_parent();
	return (0);
}
