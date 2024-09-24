#include "minishell.h"


//special build-ins are cd, export, unset, exit because they dont go into child process
bool	special_buildin(int ac, char **av)
{
	char	*cmd;
	int	len;

	cmd = av[1]; //here it should be command from parser actually
	len = ft_strlen(cmd);
	if (((len == 6) && !(ft_strncmp("export", cmd, len + 1)) && ac != 2) //instead of ac should be smth else, idea is that export with arguments only works
			|| ((len == 2) && !ft_strncmp("cd", cmd, len + 1))
			|| ((len == 5) && !ft_strncmp("unset", cmd, len + 1))
			|| ((len == 4) && !ft_strncmp("exit", cmd, len + 1)))
		return (true);
	return (false);
}


//again instead of av we have cmds
void	execute_buildin(t_shell *ms, char **av)
{
	//mb some checks before ??
	if (!ft_strncmp("echo", av[1], 5))
		buildin_echo(ms, av);
	else if (!ft_strncmp("cd", av[1], 3))
		buildin_cd(ms, av[2]);//i need the second one to chek if we go home or ..
	else if (!ft_strncmp("env", av[1], 4))
		buildin_env(ms, 0, 0);
	else if (!ft_strncmp("pwd", av[1], 4))
		buildin_pwd(ms);
	else
		printf("Unknown command: %s\n", av[1]);
}
