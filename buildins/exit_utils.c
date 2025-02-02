/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:04:06 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/14 11:04:30 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	long_min_check(char *str, int i)
{
	if (str && i > 19)
		return (1);
	if (i == 19 && ft_strncmp(str, "9223372036854775808", 20) > 0)
		return (1);
	return (0);
}

int	long_max_check(char *str, int i)
{
	if (i > 19)
		return (1);
	if (i == 19 && ft_strncmp(str, "9223372036854775807", 20) > 0)
		return (1);
	return (0);
}
