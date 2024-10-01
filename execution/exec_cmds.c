#include "../includes/minishell.h"
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>


char	*build_executable(t_ast *node, t_shell *ms);
bool	is_buildin(t_ast *ast);
int	exec_buildin(t_shell *ms, t_ast *ast);
int	redirect(t_shell *ms, t_ast *ast, int cmd_id, int *prev_fd);
void	minishell_close(int *fd);
int	command_id(t_ast *cmd, t_ast *prev, int reset);
int	commands_can_continue(t_shell *ms, t_ast *root, t_ast *limit);
void	commands_wait(t_shell *ms, t_ast *ast, t_ast *limit);


/////PROBLEM/////
/*echo hello works as expected:
It correctly forks, prints "hello," and exits with code 0.
grep hello is forked, but doesn't seem to produce any output:
The process is created, but it doesn’t seem to process the input from the previous command (echo).
This means there is likely a problem with either:

The pipe’s redirection between the two commands.
File descriptor management (closing and redirecting the correct ends of the pipe).*/



/**
 * exec_bin - Executes a command using execve.
 * @ms: A pointer to the shell structure containing environment variables.
 * @node: The AST node representing the command to execute.
 *
 * This function builds the executable path using build_executable() 
 * and attempts to execute the command using execve. If execve fails,
 * an error message is printed and the exit code is set.
 *
 * Returns:
 *   The return value of execve (0 on success, -1 on failure).
 */
static int	exec_bin(t_shell *ms, t_ast *node)
{
	int	ret;
	char	*cmd_path;

	ret = 0;
	cmd_path = build_executable(node, ms);

	if (cmd_path)
	{
		ret = execve(cmd_path, node->exp_value, ms->my_envp);
		if (ret == -1)
		{
			perror("execve");
			ms->exit_code = 1;
			exit(ms->exit_code);  // Exit the child process after failure
		}
	}
	else
	  {
		// Command path not found or build_executable failed
		fprintf(stderr, "Command not found: %s\n", node->value);
		ms->exit_code = 127;  // Set exit code to 127 (command not found)
		exit(ms->exit_code);  // Ensure child process exits on failure
		}
	return (ret);
}

/**
 * exec_fork - Forks a process to execute a command.
 * @ms: A pointer to the shell structure containing environment variables.
 * @ast: The AST node representing the command to execute.
 * @cmd_id: The command ID for the execution.
 *
 * This function forks a new process to execute the specified command
 * represented by the AST node. It also handles built-in commands and 
 * executes the command with appropriate redirection.
 */
static void	exec_fork(t_shell *ms, t_ast *ast, int cmd_id, int *prev_fd)
{
	bool	buildin;

	ast->pid = fork();
	printf("Forking process for command '%s', pid = %d\n", ast->value, ast->pid);
	if (ast->pid < 0)
	{
		perror("fork");
		minishell_close(ms->pipe);
		minishell_close(prev_fd);
		return ;
	}

	printf("Before forking for command: %s\n", ast->value);
	if (ast->pid == 0)//child process
	{
		buildin = is_buildin(ast);
		printf("Executing %s command in child process\n", buildin ? "built-in" : "external");
		
		if (redirect(ms, ast, cmd_id, prev_fd) != 0)
			exit(ms->exit_code); // Exit if redirection fails
		
		//close fds
		if (buildin)
		{
			printf("exec build in\n");
			exec_buildin(ms, ast);
		}
		else
		{
			printf("exec bin\n");
			exec_bin(ms, ast);
		}
		
		minishell_close(ms->pipe);
		exit(ms->exit_code);
	}

}

/**
 * exec_pipe - Executes a command in a piped environment.
 * @ms: A pointer to the shell structure containing environment variables.
 * @cmd: The AST node representing the command to execute.
 * @cmd_id: The command ID for the execution.
 *
 * This function manages the creation of pipes and forks processes to
 * execute commands that are part of a pipeline. It handles reading from
 * previous commands and writing to subsequent commands.
 */
static void	exec_pipe(t_shell *ms, t_ast *cmd, int cmd_id)
{
	int		prev_fd[2];

	// Initialize the pipe and file descriptors
	cmd->pid = -1;
	prev_fd[0] = ms->pipe[0];
	prev_fd[1] = ms->pipe[1];


	if (cmd_id < CMD_LAST)//if cmd->rigth still exist then its not the end
	{
		if (pipe(ms->pipe) == -1)  // Create a new pipe
		{
		    perror("pipe");
		    minishell_close(prev_fd); // Close the previous pipe's read end
		    return ;
        	}
		printf("Pipe created: [%d, %d]\n", ms->pipe[0], ms->pipe[1]);
	}

	// Fork and execute the command
	exec_fork(ms, cmd, cmd_id, prev_fd);
	
	 // Close the write end of the new pipe in the parent process
	    if (cmd_id < CMD_LAST && ms->pipe[1] >= 0)
	    {
		    printf("Parent closing write end of pipe for '%s'\n", cmd->value);
		    close(ms->pipe[1]);
		    ms->pipe[1] = -1;
	    }

	    // Close the read end of the previous pipe in the parent process
	    if (cmd_id > CMD_FIRST && prev_fd[0] >= 0)
	    {
		    printf("Parent closing read end of previous pipe for '%s'\n", cmd->value);
		    close(prev_fd[0]); // Close the read end of the previous pipe in the parent
		    
	    }
}

/**
 * exec_command - Executes a command based on the provided AST node.
 * @ms: A pointer to the shell structure containing environment variables.
 * @cmd: The AST node representing the command to execute.
 * @cmd_id: The command ID for the execution.
 *
 * This function checks if the command is a built-in command and executes
 * it accordingly. It also determines whether to execute the command with
 * or without piping.
 */
void	exec_command(t_shell *ms, t_ast *cmd, int cmd_id)
{
	bool	buildin;

	if (!cmd || cmd->exp_value[0] == NULL)
        	return ;
	buildin = is_buildin(cmd);
	/*if (buildin && cmd_id == CMD_NOPIPE)
	{
	
		if (redirect(ms, cmd, cmd_id, NULL))
			exec_buildin(ms, cmd);
	
	}*/

	printf(" cmd id = %d\n", cmd_id);
	if (cmd_id == CMD_NOPIPE)
		exec_fork(ms, cmd, cmd_id, NULL);
	else
	{
		printf("do pipe\n");
		exec_pipe(ms, cmd, cmd_id);
	}
}

/**
 * commands_exec - Executes commands represented by an AST recursively.
 * @ms: A pointer to the shell structure containing environment variables.
 * @ast: The current AST node representing the command to execute.
 * @prev: The previous AST node, used to manage command execution flow.
 *
 * This function recursively traverses the AST, executing each command
 * in the appropriate order based on the structure of the AST. It manages
 * command identifiers for proper execution.
 */
void	commands_exec(t_shell *ms, t_ast *ast, t_ast *prev)
{
	static t_ast	*root;
	int	id;

	if (!ast)
		return ;

	printf("Current command: %s, Previous command type: %d\n", ast->value, prev->type);

	if (ast->type == T_CMND_AST &&  ast->exp_value)
	{
		id = command_id(ast, prev, 0);
		if (id == CMD_LAST)
			command_id(ast, prev, 1);
		exec_command(ms, ast, id);
	}
	if (ast->left)
		commands_exec(ms, ast->left, ast);
	if ((ast->right) && (commands_can_continue(ms, ast, ast->right)))
		commands_exec(ms, ast->right, ast);
	commands_wait(ms, ast, NULL);
}*/

