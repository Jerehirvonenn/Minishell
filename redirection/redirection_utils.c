/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 16:17:30 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/13 16:26:36 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error_options(int error)
{
	if (error == 1)
		ft_putstr_fd(": Permission denied", 2);
	else if (error == 2)
		ft_putstr_fd(": No such file or directory", 2);
	else if (error == 3)
		ft_putstr_fd("Command not found: ", 2);
}

void	error_handler(char *file_name, int error)
{
	ft_putstr_fd("minishell: ", 2);
	if (file_name)
		ft_putstr_fd(file_name, 2);
	error_options(error);
	ft_putstr_fd("\n", 2);
}

int	check_file_access(char *filename, int access_mode, int error_code)
{
	if (access(filename, access_mode))
	{
		error_handler(filename, error_code);
		return (-1);
	}
	return (0);
}
