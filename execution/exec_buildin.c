#include "../includes/minishell.h"

/*
Your shell must implement the following builtins:
* echo with option -n
* cd with only a relative or absolute path &
* pwd with no options
◦ export with no options &
◦ unset with no options &
* env with no options or arguments
*  exit with no options & */

int	buildin_cd(t_shell *ms, char *cmd);
void	builtin_exit(t_shell *ms, char **cmd);

bool	is_buildin(t_ast *ast)
{
	char	*cmd;
	int		len;

	cmd = ast->exp_value[0]; //executable command
	len = ft_strlen(cmd);
	if (((len == 6) && !(ft_strncmp("export", cmd, len + 1))) //export with arguments only works
		|| ((len == 2) && !ft_strncmp("cd", cmd, len + 1))
		|| ((len == 5) && !ft_strncmp("unset", cmd, len + 1))
		|| ((len == 4) && !ft_strncmp("exit", cmd, len + 1)))
		return (true);
	return (false);
}

int	exec_buildin(t_shell *ms, t_ast *ast)
{
	int	ret;

	ret = 0;
	printf("Command received: %s\n", ast->exp_value[0]);
	//mb some checks before ??
	if (!ft_strncmp("echo", ast->exp_value[0], 5))
		buildin_echo(ms, ast->exp_value);
	else if (!ft_strncmp("cd", ast->exp_value[0], 3))
	{
		printf("Entering cd command\n");
		ret = buildin_cd(ms, ast->exp_value[1]);
		printf("ret = %d\n", ret);
	}
	//i need the second one to chek if we go home or ..
	else if (!ft_strncmp("env", ast->exp_value[0], 3))
	{
		printf("Entering env command\n");
		buildin_env(ms, 0, 0);
	}
	else if (!ft_strncmp("pwd", ast->exp_value[0], 4))
	{
		printf("Entering pwd command\n");
		printf("%s\n", ms->pwd);
	}
	else if (!ft_strncmp("exit", ast->exp_value[0], 5))
	{
		printf("Entering exit command\n");
        builtin_exit(ms, ast->exp_value);
	}
	else
		fprintf(stderr, "Unknown command: %s\n", ast->exp_value[0]);
	ms->exit_code = ret;
	return (1);
}

int	exec_bin(t_shell *ms, t_ast *node)
{
	int		ret;
	char	*cmd_path;

	ret = 0;

	// Handle redirections before executing the binary
	if (node->io_list)
	{
		fprintf(stderr, "redirection");
		if (redirection(node->io_list) != 0)
		{
			ms->exit_code = 1;  // Set exit code on redirection failure
			return (1);  // Exit early if redirection fails
		}
	}
	cmd_path = build_executable(node, ms);
	if (cmd_path)
	{
		ret = execve(cmd_path, node->exp_value, ms->my_envp);
		if (ret == -1)
		{
			perror("execve");
			exit(1); // Exit the child process after failure
		}
	}
	else
	{
		fprintf(stderr, "Command not found: %s\n", node->value);
		exit(127); // Ensure child process exits on failure
	}
	return (ret);
}

// Function to handle command execution in the child process
void	child_process(t_shell *ms, t_ast *ast)
{
	    bool buildin;
		
		buildin = is_buildin(ast);
		fprintf(stderr, "In child_process, executing command: %s\n", ast->exp_value[0]);
	   
		if (buildin)
		    exec_buildin(ms, ast);
	    else
		    exec_bin(ms, ast);
	    // Child process must exit after executing the command
	    //fprintf(stderr, "Child process exiting with exit code %d\n", ms->exit_code);
	    //exit(ms->exit_code);
		exit(1);
}

