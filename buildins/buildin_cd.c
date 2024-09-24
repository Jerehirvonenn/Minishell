
#include "minishell.h"

void	init_envp(t_shell *ms);

//in thi sfunction we get the path to a certain envp var
static char	*find_the_path(char *str, t_shell *ms)
{
	int	i;

	i = 0;
	while (ms->my_envp[i])
	{
		//if we find the match, we should return the pointer to the rest of the line after =
		if (!ft_strncmp(ms->my_envp[i], str, ft_strlen(str)))
			return (ft_substr(ms->my_envp[i], ft_strlen(str), ft_strlen(ms->my_envp[i]) - ft_strlen(str)));//ft_substr malloc space!!
		i++;
	}
	return (NULL);
}

//we change the dir to the specific path
static int	change_dir_path(t_shell *ms, char *str)
{
	char	*tmp;
	int	ret;

	tmp = find_the_path(str, ms);
	if (tmp == NULL) 
	{
    		fprintf(stderr, "%s not found\n", str);
    		return -1; // or some error code
	}
	//i need a failure protection
	ret = chdir(tmp);
	free(tmp);
	//protection if ret hence chdir fails!!
	if (ret != 0)
	{
		str = ft_substr(str, 0, ft_strlen(str) - 1);
		ft_putstr_fd(str, STDERR_FILENO);
		free(str);
		ft_putendl_fd(" not set", STDERR_FILENO);
	}

	return (ret);//its eather 0sucess 1-error
}

//upadete pwds
static void	update_pwds(t_shell *ms)
{
	char	*tmp;

	//store the old PWD
	tmp = ft_strdup(ms->pwd);
	free(ms->old_pwd);
	ms->old_pwd = tmp;
	free(ms->pwd);

	//get the curernt working directory 
	ms->pwd = getcwd(NULL, 0);
	// Handle error
}

int	buildin_cd(t_shell *ms, char *cmd)
{
	int	ret;

	//if cmd is just cd or cd ~ -> go to home dir
	// cmd is text after cd
	//MB SOMETHING ABOUT SPACES!!!!!
	if (!cmd || (ft_strncmp(cmd, "~", 2) == 0))
		ret = change_dir_path(ms, "HOME=");
	//if its - or .. then we go to the OLDPWD
	else if ((ft_strncmp(cmd, "-", 2) == 0) || (ft_strncmp(cmd, "..", 3) == 0))
		ret = change_dir_path(ms, "OLDPWD=");
	else
		ret = chdir(cmd);
	//need to check if it fails
	if (ret != 0)
	{
		printf("c error -> mistake\n");
		return (EXIT_FAILURE);
	}
	update_pwds(ms);
	//for tests:
    	printf("PWD: %s\n", ms->pwd);

	return (EXIT_SUCCESS);
}

/*int main(int ac, char **av) 
{
    t_shell ms;

    // Initialize envp
    ms.envp_size = 0;
    init_envp(&ms);

    ms.pwd = getcwd(NULL, 0); // Initial PWD
    ms.old_pwd = ft_strdup(""); // Initial OLDPWD
    
    // Test cases
    printf("Current PWD: %s\n", ms.pwd);

    // Test changing to home directory
    printf("Changing to home directory...\n");
    //mini_cd(&ms, "~");
    buildin_cd(&ms, NULL);
    printf("Current PWD after cd ~: %s\n", ms.pwd);

    // Test changing to old directory
    printf("Changing to old directory...\n");
    buildin_cd(&ms, "-");
    printf("Current PWD after cd -: %s\n", ms.pwd);

    // Change to a valid directory 
    printf("Changing to /bin...\n");
    buildin_cd(&ms, "/bin");
    printf("Current PWD after cd /bin: %s\n", ms.pwd);

    // Change to an invalid directory
    printf("Changing to an invalid directory...\n");
    buildin_cd(&ms, "/invalid/path");
    printf("Current PWD after cd /invalid/path: %s\n", ms.pwd);

    // Cleanup
    free(ms.pwd);
    free(ms.old_pwd);

    return 0;
}*/
