/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:52:01 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/13 17:09:45 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	ft_isredirection(enum e_token_type type)
{
	return (type == T_IN_REDIR || type == T_OUT_REDIR
		|| type == T_HERE_DOC || type == T_APEND);
}

t_io_type	token_to_io_type(t_token_type type)
{
	if (type == T_IN_REDIR)
		return (T_IN);
	else if (type == T_OUT_REDIR)
		return (T_OUT);
	else if (type == T_HERE_DOC)
		return (T_HEREDOC);
	else
		return (T_APPEND);
}

const char	*token_type_to_str(t_token_type type)
{
	if (type == T_PIPE)
		return ("|");
	else if (type == T_IN_REDIR)
		return ("<");
	else if (type == T_OUT_REDIR)
		return (">");
	else if (type == T_CMND)
		return ("CMND");
	else if (type == T_HERE_DOC)
		return ("<<");
	else if (type == T_APEND)
		return (">>");
	else
		return ("UNKNOWN");
}

int	heredoc_write(t_ms *ms, char *line, int fd)
{
	if (write(fd, line, ft_strlen(line)) == -1)
	{
		ft_putstr_fd("minishell: error writing to Heredoc", 2);
		ms->stop = 1;
		return (1);
	}
	if (write(fd, "\n", 1) == -1)
	{
		ft_putstr_fd("minishell: error writing to Heredoc", 2);
		ms->stop = 1;
		return (1);
	}
	return (0);
}
