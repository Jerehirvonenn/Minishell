#include "../includes/minishell.h"

//in thi sfunction we get the path to a certain envp var
static char	*find_the_path(char *str, t_ms *ms)
{
	int	i;

	i = 0;
	while (ms->my_envp[i])
	{
		//if we find the match, we should return the pointer to the rest of the line after =
		if (!ft_strncmp(ms->my_envp[i], str, ft_strlen(str)))
			return (ft_substr(ms->my_envp[i], ft_strlen(str), ft_strlen(ms->my_envp[i]) - ft_strlen(str)));
		i++;
	}
	return (NULL);
}

static int change_dir_path(t_ms *ms, char *str)
{
    char *tmp;
    int ret;
    char *current_pwd;

    // Store the old PWD before changing directory
    current_pwd = getcwd(NULL, 0);
    if (current_pwd == NULL)
    {
        perror("getcwd");
        return -1;
    }

    tmp = find_the_path(str, ms);  // Find the path using environment variables
    if (tmp == NULL)
    {
        fprintf(stderr, "%s not found\n", str);
        free(current_pwd);  // Free the old PWD
        return -1;  // Return error if not found
    }

    printf("Changing directory to: %s\n", tmp);

    // Change directory
    ret = chdir(tmp);
    free(tmp);  // Free the temporary string allocated by find_the_path

    // If chdir() fails, handle the error
    if (ret != 0)
    {
        fprintf(stderr, "%s not set\n", str);
        free(current_pwd);  // Free the old PWD before returning
        return ret;  // Return error code
    }

    // Store the current PWD in ms->old_pwd
    free(ms->old_pwd);  // Free the previous OLD PWD if it exists
    ms->old_pwd = current_pwd;  // Set OLD PWD to the current directory before the change

    // Get the new PWD after changing the directory
    ms->pwd = getcwd(NULL, 0);  // Get the new PWD using getcwd()

    // If getcwd fails, handle the error
    if (ms->pwd == NULL)
    {
        perror("getcwd");
        return -1;
    }

    return (ret);  // Return 0 on success
}


//upadete pwds
static void	update_pwds(t_ms *ms)
{
	char	*tmp;

	//store the old PWD
	tmp = ft_strdup(ms->pwd);
	free(ms->old_pwd);
	ms->old_pwd = tmp;
	free(ms->pwd);

	//get the curernt working directory 
	ms->pwd = getcwd(NULL, 0);
    if (ms->pwd == NULL) 
    {
        perror("getcwd failed");
        exit(EXIT_FAILURE);
    }
}

int	builtin_cd(t_ms *ms, char *cmd)
{
	int	ret;

	//if cmd is just cd or cd ~ -> go to home dir
	// cmd is text after cd
	//MB SOMETHING ABOUT SPACES!!!!!
    fprintf(stderr, "Current directory before cd: %s\n", ms->pwd);
	if (!cmd || (ft_strncmp(cmd, "~", 2) == 0))
    {
        printf("Changing to home directory\n");
		ret = change_dir_path(ms, "HOME=");
    }
	//if its - or .. then we go to the OLDPWD
	else if (ft_strncmp(cmd, "-", 2) == 0)
	{
        if (ms->old_pwd != NULL)
        {
            printf("Changing to previous directory: %s\n", ms->old_pwd);
            ret = change_dir_path(ms, "OLDPWD=");
        }
        else
        {
            fprintf(stderr, "OLDPWD not set\n");
            return (EXIT_FAILURE);
        }
    }
    else if (ft_strncmp(cmd, "..", 3) == 0)
    {
        fprintf(stderr, "Changing to parent directory\n");
        ret = chdir("..");
    }
	else
    {
        fprintf(stderr, "Changing to directory: %s\n", cmd);
		ret = chdir(cmd);
    }
	//need to check if it fails
	if (ret != 0)
	{
		fprintf(stderr, "c error -> mistake\n");
		return (EXIT_FAILURE);
	}
	update_pwds(ms);
	//for tests:
    fprintf(stderr, "PWD after cd: %s\n", ms->pwd);

	return (EXIT_SUCCESS);
}
