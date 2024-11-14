/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:16:21 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/14 18:24:00 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../Libft/libft.h"
# include "struct.h"
# include "token.h"
# include "parsing.h"
# include "execution.h"
# include "termios.h"

extern int	ms_signal;

//writing_utils.c
void	error_msg(char *first_line, char *cmd, char *second_line);

//clean.c
void	free_array(char **array);
void	clean_ms(t_ms *ms);

//signals
int		signal_handler_child(void);
int		signal_handler_exec(void);
int		signal_handler_parent(void);
int		signal_handler_heredoc(t_ms *ms);

//main_utils.c
void	init_minishell(t_ms *ms, char **envp);
void	reset_ms(t_ms *ms);
int		just_whitespace(char *str);

#endif
