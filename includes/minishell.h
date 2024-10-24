#ifndef MINISHELL_H
# define MINISHELL_H

extern int ms_signal;

#include "../Libft/libft.h"
#include "struct.h"
#include "token.h"
#include "parsing.h"
#include "execution.h"
#include <stdio.h>
#include <stdlib.h>

void	signal_handler();
void	free_array(char **array);
void	clean_ms(t_ms *ms);

#endif
