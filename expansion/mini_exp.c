/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:00:08 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/13 13:15:11 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	clean_empty_exp(t_ms *ms, t_ast *node)
{
	char	*expansion;
	int		i;

	i = 0;
	while (node->exp_value[i])
	{
		if (!*node->exp_value[i] || ft_strchr(node->exp_value[i], '\'')
			|| ft_strchr(node->exp_value[i], '\"'))
		{
			i++;
			continue ;
		}
		ms->tmp1 = ft_strdup(node->exp_value[i]);
		if (!ms->tmp1)
			malloc_parent_failure(ms);
		expansion = expand_argument(ms->tmp1, ms);
		free(ms->tmp1);
		ms->tmp1 = NULL;
		if (!*expansion)
			remove_string_i(node->exp_value, i);
		else
			i++;
		free(expansion);
	}
}

static void	replace_cmnd(t_ms *ms, t_ast *node)
{
	free(node->value);
	if (node->exp_value && node->exp_value[0])
	{
		node->value = ft_strdup(node->exp_value[0]);
		if (!node->value)
			malloc_parent_failure(ms);
		node->empty = 0;
	}
	else
	{
		node->value = ft_strdup("");
		if (!node->value)
			malloc_parent_failure(ms);
		node->exp_value[0] = ft_strdup("");
		if (!node->exp_value[0])
			malloc_parent_failure(ms);
		node->empty = 1;
	}
}

static void	check_cmnd_change(t_ms *ms, t_ast *node)
{
	char	*tmp;
	char	*expansion;

	if (!*node->value || ft_strchr(node->value, '\'')
		|| ft_strchr(node->value, '\"'))
		return ;
	tmp = ft_strdup(node->value);
	if (!tmp)
		malloc_parent_failure(ms);
	ms->tmp1 = tmp;
	expansion = expand_argument(tmp, ms);
	free(tmp);
	ms->tmp1 = expansion;
	if (!*expansion)
		replace_cmnd(ms, node);
	free(expansion);
	ms->tmp1 = NULL;
}

static void	check_ambigious(t_ms *ms, t_io *io)
{
	char	*tmp;
	char	*expansion;

	while (io)
	{
		if (io->type == T_HEREDOC)
			io = io->next;
		else
		{
			tmp = ft_strdup(io->value);
			if (!tmp)
				malloc_parent_failure(ms);
			ms->tmp1 = tmp;
			expansion = expand_argument(tmp, ms);
			free(tmp);
			ms->tmp1 = NULL;
			if (!*expansion)
				io->amb_exp = 1;
			else
				io->amb_exp = 0;
			free(expansion);
			io = io->next;
		}
	}
}

void	mini_exp(t_ms *ms, t_ast *node)
{
	if (!node)
		return ;
	else if (node->type == T_CMND)
	{
		if (node->value && node->exp_value)
		{
			clean_empty_exp(ms, node);
			check_cmnd_change(ms, node);
		}
		if (node->io_list)
		{
			check_ambigious(ms, node->io_list);
		}
	}
	if (node->left)
		mini_exp(ms, node->left);
	if (node->right)
		mini_exp(ms, node->right);
}
