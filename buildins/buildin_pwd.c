#include "minishell.h"

void	buildin_pwd(t_shell *ms)
{
	char *cwd;

	cwd = getcwd(NULL, 0);//it allocate itself
	if (cwd)
	{
		ft_putendl_fd(cwd, 1);
		free(cwd);
		return ;//mb exit code in the struct??
	}
	else
		ft_putendl_fd(ms->pwd, 1);//??print previous one if it fails???
}

/*// Test function
int	main(int ac, char **av, char **envp)
{
	t_shell ms;

	ms.envp_size = 0;
	ms.pwd = getcwd(NULL, 0);
	ms.old_pwd = ft_strdup("");

	printf("Test 1: Get current working directory:\n");
	mini_pwd(&ms);

}*/
