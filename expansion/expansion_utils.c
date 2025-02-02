/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:56:04 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/13 13:47:29 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isquote(char c)
{
	return (c == '\"' || c == '\'');
}

int	ft_isdelim(char c)
{
	return (c == ' ' || c == '\t');
}

void	remove_string_i(char **array, int i)
{
	int	size;

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
