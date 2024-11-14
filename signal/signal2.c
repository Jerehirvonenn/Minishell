/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:54:51 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/14 11:55:15 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_sigint_exec(int num)
{
	ms_signal = num;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	ft_sigquit(int num)
{
	ms_signal = num;
	printf("Quit (core dumped)\n");
}

void	signal_handler_exec(void)
{
	signal(SIGINT, ft_sigint_exec);
	signal(SIGQUIT, ft_sigquit);
}

void	signal_handler_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
