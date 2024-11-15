/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:55:19 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/15 09:18:12 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_sigint_parent(int num)
{
	g_signal = num;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_sigint_heredoc(int num)
{
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_done = 1;
	g_signal = num;
}

int	ft_readline_event(void)
{
	return (0);
}

int	signal_handler_heredoc(t_ms *ms)
{
	rl_event_hook = ft_readline_event;
	if (signal(SIGINT, ft_sigint_heredoc) == SIG_ERR)
	{
		ft_putstr_fd("minishell: failed to set signal handlers\n", 2);
		clean_ms(ms);
		ft_free_ast(ms->ast);
		exit(1);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		ft_putstr_fd("minishell: failed to set signal handlers\n", 2);
		clean_ms(ms);
		ft_free_ast(ms->ast);
		exit(1);
	}
	return (0);
}

int	signal_handler_parent(void)
{
	rl_done = 0;
	rl_event_hook = NULL;
	if (signal(SIGINT, ft_sigint_parent) == SIG_ERR)
	{
		ft_putstr_fd("minishell: failed to set signal handlers\n", 2);
		return (-1);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		ft_putstr_fd("minishell: failed to set signal handlers\n", 2);
		return (-1);
	}
	return (0);
}
