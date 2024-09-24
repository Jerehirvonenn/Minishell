# ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

#include "libft/libft.h"


typedef struct s_shell
{
	char	**my_envp;
	int	envp_size;
	int	excode;
	char	*pwd;
	char	*old_pwd;//??
}	t_shell;

void	init_shell(t_shell *ms);

//buildins
int	buildin_cd(t_shell *ms, char *cmd);
void	buildin_echo(t_shell *ms, char **cmd);
void	buildin_env(t_shell *ms, int i, int j);
void	buildin_pwd(t_shell *ms);

#endif
