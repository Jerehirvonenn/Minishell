/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:20:45 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/13 11:06:41 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error_msg(char *first_line, char *cmd, char *second_line)
{
	ft_putstr_fd(first_line, 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(second_line, 2);
}
