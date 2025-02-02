/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_buildin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:15:41 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/14 18:56:45 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_builtin(t_ast *ast)
{
	char	*cmd;
	int		len;

	if (!ast->value)
		return (false);
	cmd = ast->exp_value[0];
	len = ft_strlen(cmd);
	if (((len == 6) && !(ft_strncmp("export", cmd, len + 1)))
		|| ((len == 2) && !ft_strncmp("cd", cmd, len + 1))
		|| ((len == 5) && !ft_strncmp("unset", cmd, len + 1))
		|| ((len == 4) && !ft_strncmp("exit", cmd, len + 1)))
		return (true);
	return (false);
}

bool	is_child_builtin(t_ast *ast)
{
	char	*cmd;
	int		len;

	cmd = ast->exp_value[0];
	len = ft_strlen(cmd);
	if (((len == 3) && !(ft_strncmp("pwd", cmd, len + 1)))
		|| ((len == 3) && !ft_strncmp("env", cmd, len + 1))
		|| ((len == 4) && !ft_strncmp("echo", cmd, len + 1)))
		return (true);
	return (false);
}

int	check_env_argument(t_ast *ast, t_ms *ms)
{
	if (ast->exp_value[1])
	{
		error_msg("env: '", ast->exp_value[1],
			"': No such file or directory\n");
		ms->exit_code = 127;
		return (-1);
	}
	return (0);
}

int	exec_builtin(t_ms *ms, t_ast *ast)
{
	int	ret;

	ret = 0;
	if (!ft_strncmp("echo", ast->exp_value[0], 5))
		builtin_echo(ast->exp_value);
	else if (!ft_strncmp("cd", ast->exp_value[0], 3))
		ret = builtin_cd(ms, ast, ast->exp_value[1]);
	else if (!ft_strncmp("env", ast->exp_value[0], 3))
	{
		if (check_env_argument(ast, ms) == -1)
			return (-1);
		builtin_env(ms);
	}
	else if (!ft_strncmp("pwd", ast->exp_value[0], 4))
		printf("%s\n", ms->pwd);
	else if (!ft_strncmp("exit", ast->exp_value[0], 5))
		builtin_exit(ms, ast->exp_value);
	else if (!ft_strncmp("export", ast->exp_value[0], 7))
		builtin_export(ms, ast->exp_value, 1);
	else if (!ft_strncmp("unset", ast->exp_value[0], 6))
		builtin_unset(ms, ast->exp_value);
	else
		printf("Command not found: %s\n", ast->exp_value[0]);
	return (ret);
}

// Function to handle command execution in the child process
void	child_process(t_ms *ms, t_ast *ast)
{
	if (signal_handler_child() == -1)
		clean_and_exit(ms, 1);
	if (ast->empty == 1 || !ast->value)
		clean_and_exit(ms, 0);
	if (is_child_builtin(ast) || is_builtin(ast))
	{
		if (exec_builtin(ms, ast) == -1)
			clean_and_exit(ms, ms->exit_code);
		ft_free_ast(ms->ast);
		clean_ms(ms);
	}
	else
	{
		if (exec_bin(ms, ast) == -1)
			clean_and_exit(ms, ms->exit_code);
	}
}
