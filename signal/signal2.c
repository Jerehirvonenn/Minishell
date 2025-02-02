/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:54:51 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/15 09:18:30 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_sigint_exec(int num)
{
	g_signal = num;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	ft_sigquit(int num)
{
	g_signal = num;
	printf("Quit (core dumped)\n");
}

int	signal_handler_exec(void)
{
	if (signal(SIGINT, ft_sigint_exec) == SIG_ERR)
	{
		ft_putstr_fd("minishell: failed to set signal handlers\n", 2);
		return (-1);
	}
	if (signal(SIGQUIT, ft_sigquit) == SIG_ERR)
	{
		ft_putstr_fd("minishell: failed to set signal handlers\n", 2);
		return (-1);
	}
	return (0);
}

int	signal_handler_child(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
	{
		ft_putstr_fd("minishell: failed to set signal handlers\n", 2);
		return (-1);
	}
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
	{
		ft_putstr_fd("minishell: failed to set signal handlers\n", 2);
		return (-1);
	}
	return (0);
}
