/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmnd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:43:23 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/13 18:00:54 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_redirection(t_token **tokens, t_ms *ms, t_parsing *data, t_ast *node)
{
	if ((*tokens)->next && (*tokens)->next->type == T_CMND)
	{
		if (add_io(node,
				token_to_io_type((*tokens)->type),
				(*tokens)->next->value, tokens))
			parsing_malloc_failure(ms, data, node);
	}
	else
	{
		if (ft_isredirection((*tokens)->type))
			*tokens = (*tokens)->next;
		ft_free_ast_node(node);
		return (0);
	}
	return (1);
}

void	app_cmnd_arg(t_token **tokens, t_ms *ms, t_parsing *data, t_ast *node)
{
	node->exp_value = append_args(node->exp_value,
			(*tokens)->value, ms, tokens);
	if (!node->exp_value)
		parsing_malloc_failure(ms, data, node);
}

void	set_cmnd_val(t_token **tokens, t_ms *ms, t_parsing *data, t_ast *node)
{
	node->value = ft_strdup((*tokens)->value);
	if (!node->value)
		parsing_malloc_failure(ms, data, node);
}

int	cmnd_and_redir(t_token **tokens, t_ms *ms, t_parsing *data, t_ast *node)
{
	while (*tokens && ((*tokens)->type == T_CMND
			|| ft_isredirection((*tokens)->type)))
	{
		if ((*tokens)->type == T_CMND)
		{
			if (!node->value)
				set_cmnd_val(tokens, ms, data, node);
			app_cmnd_arg(tokens, ms, data, node);
		}
		else if (ft_isredirection((*tokens)->type))
		{
			if (!handle_redirection(tokens, ms, data, node))
				return (0);
		}
		else
		{
			if (ft_isredirection((*tokens)->type))
				*tokens = (*tokens)->next;
			ft_free_ast_node(node);
			return (0);
		}
	}
	return (1);
}

t_ast	*parse_command(t_token **tokens, t_ms *ms, t_parsing *data)
{
	t_ast	*node;

	if ((*tokens)->type == T_CMND)
		node = create_ast_node(T_CMND, (*tokens)->value);
	else
		node = create_ast_node(T_CMND, NULL);
	if (!node)
		parsing_malloc_failure(ms, data, NULL);
	if (node->value)
	{
		node->exp_value = append_args(node->exp_value,
				(*tokens)->value, ms, tokens);
		if (!node->exp_value)
			parsing_malloc_failure(ms, data, node);
	}
	if (!cmnd_and_redir(tokens, ms, data, node))
		return (NULL);
	return (node);
}
