/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhirvone <jhirvone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:00:08 by jhirvone          #+#    #+#             */
/*   Updated: 2024/11/08 17:25:06 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//go through ast tree and check if $EXPANSION_DOESNT_EXIST
//if it doesnt then remove the whole pointer form expanded arguments
//if nodes main command expands to nothing then replace it from exp_values if possible.

//maybe do to io list also to catch ambigious redirection

void	remove_string_i(t_ms *ms, char **array, int i)
{
	int	size;

	(void)ms;
	size = 0;
	while (array[size])
		size++;
	free(array[i]);
	array[i] = NULL;
	while (i < size - 1)
	{
		array[i] = array[i + 1];
		i++;
	}
	array[size -1] = NULL;
}

void	clean_empty_exp(t_ms *ms, t_ast *node)
{
	int 	i;
	char	*tmp;
	char	*expansion;

	i = 0;
	while (node->exp_value[i])
	{
		//CHECK FOR ANY QUOTES THEY MAKE THE COMMAND APPEAR EVEN EMPTY
		//if its already empty it needs to stay also
		//only strings that become empty after variable expansion need to be thrown out
		if (!*node->exp_value[i] || ft_strchr(node->exp_value[i], '\'') || ft_strchr(node->exp_value[i], '\"'))
		{
			i++;
			continue ;
		}
		printf("EXPANSION BEFORE%s\n", node->exp_value[i]);
		tmp = ft_strdup(node->exp_value[i]);
		if (!tmp)
		{
			//HANDLE MALLOC FAILURE
			exit(1);
		}
		expansion = expand_argument(tmp, ms); //figure out how to handle malloc failure
		printf("EXPANSION AFTER%s\n", expansion);
		free(tmp);
		if (!*expansion)
		{
			printf("Removing bad expansion\n");
			remove_string_i(ms, node->exp_value, i);
		}
		else
			i++;
		free(expansion);
	}
}

int	replace_cmnd(t_ms *ms, t_ast *node)
{
	(void)ms;
	free(node->value);
	if (node->exp_value && node->exp_value[0])
	{
		node->value = ft_strdup(node->exp_value[0]);
	}
	else
	{
		node->value = ft_strdup("");
		node->exp_value[0] = ft_strdup("");
		//SET AST NODE TO DONT RUN MODE
	}
	return (0);
}

void	check_cmnd_change(t_ms *ms, t_ast *node)
{
	char	*tmp;
	char	*expansion;

	if (!*node->value || ft_strchr(node->value, '\'') || ft_strchr(node->value, '\"'))
		return ;

	printf("COMMAND BEFORE%s\n", node->value);
	tmp = ft_strdup(node->value);
	if (!tmp)
	{
		//HANDLE MALLOC FAILURE
		exit(1);
	}
	expansion = expand_argument(tmp, ms); //figure out how to handle malloc failure
	printf("COMMAND AFTER%s\n", expansion);
	free(tmp);
	if (!*expansion)
	{
		printf("replacing cmnd value\n");
		replace_cmnd(ms, node);
	}
	else
		free(expansion);
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
	}
	if (node->left)
		mini_exp(ms, node->left);
	if (node->right)
		mini_exp(ms, node->right);
}
