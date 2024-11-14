/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:12:12 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/14 17:12:14 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_minishell(t_ms *ms, char **envp)
{
	struct termios	term;

	ms->envp_size = 0;
	init_envp(ms, envp);
	ms->exit_code = 0;
	ms->stop = 0;
	ms->quit = 0;
	ms->heredoc = 0;
	ms->pwd = getcwd(NULL, 0);
	if (!ms->pwd)
	{
		free_array(ms->my_envp);
		ft_putstr_fd("minishell: cannot allocate memory\n", 2);
		exit(1);
	}
	ms->old_pwd = NULL;
	ms->ast = NULL;
	ms->tokens = NULL;
	ms->tmp1 = NULL;
	ms->tmp2 = NULL;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	reset_ms(t_ms *ms)
{
	ms->stop = 0;
	ms->quit = 0;
	ms->ast = NULL;
	ms->tokens = NULL;
	ms_signal = 0;
}

int	just_whitespace(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}
