#include "../includes/minishell.h"

void	signal_handler_child();

/*
Your shell must implement the following builtins:
* echo with option -n
* cd with only a relative or absolute path 
* pwd with no options
* export with no options 
◦ unset with no options 
* env with no options or arguments
*  exit with no options  */

bool	is_builtin(t_ast *ast)
{
	char	*cmd;
	int		len;

	cmd = ast->exp_value[0]; //executable command
	len = ft_strlen(cmd);
	if (((len == 6) && !(ft_strncmp("export", cmd, len + 1))) //export there is something with it??
		|| ((len == 2) && !ft_strncmp("cd", cmd, len + 1))
		|| ((len == 5) && !ft_strncmp("unset", cmd, len + 1))
		|| ((len == 4) && !ft_strncmp("exit", cmd, len + 1))
		|| ((len == 4) && !ft_strncmp("echo", cmd, len + 1))
		|| ((len == 3) && !ft_strncmp("pwd", cmd, len + 1))
		|| ((len == 3) && !ft_strncmp("env", cmd, len + 1)))
		return (true);
	return (false);
}

int	exec_builtin(t_ms *ms, t_ast *ast)
{
	int	ret;

	ret = 0;
	fprintf(stderr, "Command received: %s\n", ast->exp_value[0]);//test
	if (!ft_strncmp("echo", ast->exp_value[0], 5))
	{
		fprintf(stderr, "Entering echo command\n");//test
		builtin_echo(ms, ast->exp_value);
	}
	else if (!ft_strncmp("cd", ast->exp_value[0], 3))
	{
		fprintf(stderr, "Entering cd command\n");//test
		ret = builtin_cd(ms, ast->exp_value[1]);
		fprintf(stderr, "ret = %d\n", ret);//test
	}
	else if (!ft_strncmp("env", ast->exp_value[0], 3))
	{
		fprintf(stderr, "Entering env command\n");//test
		builtin_env(ms, 0, 0);
	}
	else if (!ft_strncmp("pwd", ast->exp_value[0], 4))
	{
		fprintf(stderr,"Entering pwd command\n");//test
		printf("%s\n", ms->pwd);
	}
	/*else if (!ft_strncmp("exit", ast->exp_value[0], 5))
	{
		fprintf(stderr, "Entering exit command\n");//test
        	builtin_exit(ms, ast->exp_value, exec);
	}*/
	else if (!ft_strncmp("export", ast->exp_value[0], 7))
	{
		fprintf(stderr, "Entering exposrt command\n");//test
		builtin_export(ms, ast->exp_value, 1);
	}
	else
		fprintf(stderr, "Unknown command: %s\n", ast->exp_value[0]);
	ms->exit_code = ret;
	return (1);
}


int	exec_bin(t_ms *ms, t_ast *node)
{
	int		ret;
	char	*cmd_path;

	ret = 0;
	cmd_path = build_executable(node, ms);
	signal_handler_child();
	if (cmd_path)
	{
		ret = execve(cmd_path, node->exp_value, ms->my_envp);
		if (ret == -1)
		{
			perror("execve");
			free(cmd_path);
			return (-1); //that means fail
		}
	}
	else
	{
		fprintf(stderr, "Command not found: %s\n", node->value);
		//free(cmd_path);
		exit(127); // Ensure child process exits on failure or set ms->exit code?
	}
	free(cmd_path);
	return (ret);
}

// Function to handle command execution in the child process
void	child_process(t_ms *ms, t_ast *ast)
{
	bool builtin;

	builtin = is_builtin(ast);
	if (builtin)
	{
		if (exec_builtin(ms, ast) != 1)
			exit(EXIT_FAILURE);
	}
	else
	{
		if (exec_bin(ms, ast) == -1)
			exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

