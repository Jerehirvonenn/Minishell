
#include "includes/minishell.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/**
 * find_executable - Searches for the full path of a command in the directories listed in PATH.
 * @cmd: The command to search for (e.g., "ls").
 * @path: The PATH environment variable, containing colon-separated directories.
 * @ms: A pointer to the shell structure containing environment variables.
 * 
 * This function splits the PATH variable into individual directories, appends
 * the command to each directory, and checks if the resulting path exists and
 * is executable using access(). If a valid path is found, it's returned.
 * 
 * Returns:
 *   A string representing the full path to the executable if found, or NULL if not found.
 */
char	*find_executable(char *cmd, char *path, t_shell *ms)
{
	char	**envp_paths;
	char	*exec;
	int		i;

	i = 0;
	envp_paths = ft_split(path, ':');
	if (!envp_paths)
		return (NULL);
	while (envp_paths[i])
	{
		exec = ft_strjoin(envp_paths[i++], cmd);
		if (!exec)
			return (NULL);//error
		if (access(exec, F_OK) == 0)
			return (exec);
		free(exec);
	}
	return (NULL);
}

/**
 * envp_exists - Searches for a specified environment variable by name.
 * @name: The name of the environment variable (e.g., "PATH").
 * @ms: A pointer to the shell structure containing environment variables.
 *
 * This function iterates through the shell's environment variables to find
 * the specified one. If found, the value after the '=' sign is returned.
 *
 * Returns:
 *   The value of the environment variable if found, or NULL if not found.
 */
char	*envp_exists(char *name, t_shell *ms)
{
	int		i;
	int		len;
	char	*temp;

	i = 0;
	temp = ft_strjoin(name, "=");
	if (!temp)
		return (NULL);
	len = ft_strlen(temp);
	while (ms->my_envp[i])
	{
		if (ms->my_envp[i] == NULL)
		{
			fprintf(stderr, "Error: my_envp[%d] is NULL\n", i);
			free(temp);
			return (NULL);
		}
		if (ft_strnstr(ms->my_envp[i], temp, len))
		{
			printf("Found: %s\n", ms->my_envp[i] + len);
			free(temp); // Free the temp variable to avoid memory leak
			return (ms->my_envp[i] + len); // Return the value after '='
		}
		i++;
	}
	free(temp); // Free temp even if not found
	return (NULL);
}

/**
 * build_executable - Constructs the full path to an executable.
 * @node: The AST node containing the command to execute.
 * @ms: A pointer to the shell structure containing environment variables.
 * 
 * This function checks if the command is an absolute or relative path. If not,
 * it uses the PATH environment variable to search for the command in the directories
 * listed in PATH. The full path is returned if found.
 * 
 * Returns:
 *   The full path to the executable if found, or NULL if not found.
 */
char	*build_executable(t_ast *node, t_shell *ms)
{
	char	*path;
	char	*binary;

	if (!node || !*node->value)
		return (NULL);
	binary = node->value; // The command itself
	if (!ft_strchr(binary, '/')) // Check if it’s a relative or absolute path
	{
		binary = ft_strjoin("/", binary);
		path = envp_exists("PATH", ms);
		if (!path)
			return (NULL);
		return (find_executable(binary, path, ms));
	}
	return (binary);
}

/**
 * exec_bin - Executes a command using execve.
 * @ms: A pointer to the shell structure containing environment variables.
 * @node: The AST node representing the command to execute.
 *
 * This function builds the executable path using build_executable() and attempts to
 * execute the command using execve. If execve fails, an error message is printed and
 * the exit code is set.
 *
 * Returns:
 *   The return value of execve (0 on success, -1 on failure).
 */
int	exec_bin(t_shell *ms, t_ast *node)
{
	int		ret;
	char	*cmd_path;

	ret = 0;
	cmd_path = build_executable(node, ms);
	printf("Command Path: %s\n", cmd_path);
	if (cmd_path)
	{
		//this 4 lines should be changed
		if (node->exp_value)
			ret = execve(cmd_path, node->exp_value, ms->my_envp);
		if (node->value)
			ret = execve(cmd_path, &node->value, ms->my_envp);
		if (ret == -1)
		{
			perror(cmd_path);
			ms->exit_code = 1;
		}
	}
	else
	{
		fprintf(stderr, "Command not found: %s\n", node->value);
		ms->exit_code = 127; // Set exit code to 127 for command not found
	}

	return (ret);
}

// Main function for testing
t_ast	*parsing_ast(t_token *tokens);
t_token *ft_tokenize(char *str);
void	init_envp(t_shell *ms);

int main(void)
{
    t_shell ms;
    t_token *tokens;
    t_ast *ast;
    char *input;
    int ret = 0;

    ms.envp_size = 0;
    init_envp(&ms);

    while (1)
    {
        input = readline("\033[33mminishell\033[0m\033[35m$\033[0m ");
        if (!input)
            break; // If input is NULL (like Ctrl+D), break the loop

        if (*input)
            add_history(input); // Add non-empty input to history
        tokens = ft_tokenize(input);
        ast = parsing_ast(tokens);
        // Execute the command (actual execution)
        exec_bin(&ms, ast);
    }
}
