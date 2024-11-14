/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:25:28 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/14 18:13:57 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <termios.h>

int	ms_signal = 0;

void		print_ast_tree(t_ast *root);
void		print_tokens(t_token *tokens);
const char	*token_type_to_str(t_token_type type);

static void	time_to_quit(t_ms *ms)
{
	ft_putstr_fd("exit\n", 1);
	clean_ms(ms);
	exit(ms->exit_code);
}

static void	signal_fail(t_ms *ms)
{
	clean_ms(ms);
	exit(1);
}

char	*check_input(t_ms *ms)
{
	char	*str;
	char	*prompt;

	if (signal_handler_parent())
		signal_fail(ms);
	reset_ms(ms);
	prompt = "minishell> ";
	str = readline(prompt);
	if (ms_signal)
	{
		ms_signal = 0;
		ms->exit_code = 130;
	}
	if (!str)
		time_to_quit(ms);
	if (!*str || just_whitespace(str))
	{
		free(str);
		ms->stop = 1;
		return (NULL);
	}
	else
		add_history(str);
	return (str);
}

void	minishell(t_ms ms)
{
	char	*str;

	while (1)
	{
		str = check_input(&ms);
		if (ms.stop)
			continue ;
		ms.tokens = ft_tokenize(str, &ms);
		if (ms.stop)
			continue ;
		ms.ast = parsing_ast(ms.tokens, &ms);
		if (ms.stop)
			continue ;
		mini_exp(&ms, ms.ast);
		expand_ast(ms.ast, &ms);
		ast_heredoc(ms.ast, &ms);
		if (ms.stop)
		{
			ft_free_ast(ms.ast);
			continue ;
		}
		execute_ast(ms.ast, &ms);
		ft_free_ast(ms.ast);
	}
	clean_ms(&ms);
}

int	main(int ac, char **av, char **envp)
{
	t_ms	ms;

	(void)ac;
	(void)av;
	init_minishell(&ms, envp);
	if(signal_handler_parent())
	{
		clean_ms(&ms);
		exit(1);
	}
	minishell(ms);
}
