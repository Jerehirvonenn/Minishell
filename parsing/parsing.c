/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:05:45 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/13 17:40:20 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**append_args(char **args, char *to_add, t_ms *ms, t_token **tokens)
{
	char	**new_args;
	int		len;
	int		i;

	len = 0;
	i = -1;
	while (args && args[len])
		len++;
	new_args = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_args)
	{
		free_array(args);
		ms->stop = 1;
		return (NULL);
	}
	while (++i < len)
		new_args[i] = args[i];
	new_args[i] = ft_strdup(to_add);
	if (!new_args[i])
	{
		free_array(args);
		free(new_args);
		ms->stop = 1;
		return (NULL);
	}
	new_args[++i] = NULL;
	free(args);
	*tokens = (*tokens)->next;
	return (new_args);
}

t_ast	*parsing_ast(t_token *tokens, t_ms *ms)
{
	t_parsing	data;

	init_parsing_struct(&data);
	if (!tokens || tokens->type == T_PIPE)
		return (parsing_error(ms, NULL, NULL, tokens));
	data.left = parse_command(&tokens, ms, &data);
	if (!data.left)
		return (parsing_error(ms, NULL, NULL, tokens));
	while (tokens && tokens->type == T_PIPE)
	{
		tokens = tokens->next;
		if (!tokens || tokens->type == T_PIPE)
			return (parsing_error(ms, data.left, NULL, tokens));
		data.right = parse_command(&tokens, ms, &data);
		if (!data.right)
			return (parsing_error(ms, data.left, NULL, tokens));
		data.pipe = create_ast_node(T_PIPE, NULL);
		if (!data.pipe)
			parsing_malloc_failure(ms, &data, NULL);
		combine_pipeline(&data);
	}
	ft_free_token(ms->tokens);
	ms->tokens = NULL;
	return (data.left);
}
