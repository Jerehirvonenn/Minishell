/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:15:31 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/14 11:06:51 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*This function ensures that the input string is numeric and checks whether 
it fits within the valid range for a long integer (LONG_MIN to LONG_MAX).*/
static int	symbol_check(char *str, int i)
{
	if ((str[0] == '-' || str[0] == '+') && ft_isdigit(str[1]))
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (1);
		i++;
	}
	if (str[0] != '+' && str[0] != '-')
		return (long_max_check(str, i));
	else if (str[0] == '+')
		return (long_max_check(str + 1, i - 1));
	else if (str[0] == '-')
		return (long_min_check(str + 1, i - 1));
	return (0);
}

// Check if the argument is numeric, and exit if not
static void	check_numeric_argument(t_ms *ms, char *arg)
{
	if (symbol_check(arg, 0) == 1)
	{
		ft_putstr_fd("exit\n", 1);
		error_msg("minishell: exit: ", arg, ": numeric argument required\n");
		ft_free_ast(ms->ast);
		clean_ms(ms);
		exit(2);
	}
}

static int	check_too_many_arguments(t_ms *ms, char **cmd)
{
	if (cmd[1] && cmd[2])
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		ms->exit_code = 1;
		return (1);
	}
	return (0);
}

void	builtin_exit(t_ms *ms, char **cmd)
{
	int	exit_code;
	int	arg;

	exit_code = 0;
	if (cmd[1])
		check_numeric_argument(ms, cmd[1]);
	if (check_too_many_arguments(ms, cmd))
		return ;
	else if (cmd[1])
	{
		arg = ft_atoi(cmd[1]);
		if (arg > -1)
			exit_code = (arg % 256);
		else
			exit_code = (256 - ((arg * -1) % 256));
	}
	if (!ms->ast->left && !ms->ast->right)
		ft_putstr_fd("exit\n", 1);
	ft_free_ast(ms->ast);
	clean_ms(ms);
	exit(exit_code);
}
