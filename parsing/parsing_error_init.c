/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error_init.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:49:46 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/14 18:35:30 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	parsing_malloc_failure(t_ms *ms, t_parsing *data, t_ast *node)
{
	ft_putstr_fd("minishell: cannot allocate memory\n", 2);
	clean_ms(ms);
	ft_free_token(ms->tokens);
	ft_free_ast(data->left);
	ft_free_ast(data->right);
	if (node)
		ft_free_ast_node(node);
	exit(1);
}

t_ast	*parsing_error(t_ms *ms, t_ast *ast, t_ast *node, t_token *token)
{
	const char	*str;

	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token)
	{
		str = token_type_to_str(token->type);
		ft_putstr_fd((char *)str, 2);
		ft_putstr_fd("\'\n", 2);
	}
	else
		ft_putstr_fd("newline'\n", 2);
	ms->stop = 1;
	ft_free_token(ms->tokens);
	ft_free_ast(ast);
	ft_free_ast(node);
	ms->exit_code = 2;
	return (NULL);
}

void	init_parsing_struct(t_parsing *data)
{
	data->left = NULL;
	data->pipe = NULL;
	data->right = NULL;
}

void	combine_pipeline(t_parsing *data)
{
	data->pipe->left = data->left;
	data->pipe->right = data->right;
	data->left = data->pipe;
	data->right = NULL;
	data->pipe = NULL;
}

void	hdoc_signal_failure(t_ms *ms, int fd)
{
	clean_ms(ms);
	ft_free_ast(ms->ast);
	if (fd != -1)
		close(fd);
	exit(1);
}
