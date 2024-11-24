/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:12:12 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/15 09:17:31 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_old_pwd(t_ms *ms)
{
	int	i;

	i = 0;
	while (ms->my_envp[i])
	{
		if (!ft_strncmp(ms->my_envp[i], "OLDPWD=", 7))
		{
			if (ft_strlen(ms->my_envp[i]) > 7)
			{
				ms->old_pwd = ft_substr(ms->my_envp[i], 7, ft_strlen(ms->my_envp[i]) - 7);
					return ;
			}
		}
		i++;
	}
}

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
	update_old_pwd(ms);
}

void	reset_ms(t_ms *ms)
{
	ms->stop = 0;
	ms->quit = 0;
	ms->ast = NULL;
	ms->tokens = NULL;
	g_signal = 0;
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
