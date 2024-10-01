#include "../includes/minishell.h"

int pid_wait(t_ast *cmd, int *exit_type);
void commands_wait(t_shell *ms, t_ast *ast, t_ast *limit);

/**
 * pid_wait - Waits for a child process to finish and retrieves its exit status.
 * @cmd: The command AST node representing the child process.
 * @exit_type: Pointer to an integer to store the type of exit (normal or signal).
 *
 * This function waits for the specified child process to terminate and captures
 * its exit status. It returns the exit code of the child process or -1 on failure.
 */
int	pid_wait(t_ast *cmd, int *exit_type)
{
	int		status;

	if (cmd->pid < 0)
		return (-1);
	if (waitpid(cmd->pid, &status, 0) < 0)
		return (-1);
	cmd->pid = -2;
	if (WIFEXITED(status))
	{
		*exit_type = 0;
		return (WEXITSTATUS(status));
	}
	if (WIFSIGNALED(status))
	{
		*exit_type = 1;
		return (WTERMSIG(status) + 128);
	}
	return (EXIT_FAILURE);
}

/**
 * commands_wait - Waits for all child processes to finish for a given AST.
 * @ms: A pointer to the shell structure containing environment variables.
 * @ast: The AST node to wait for.
 * @limit: The AST node representing
  * the limit of waiting (end of the command sequence).
 *
 * This function recursively waits for all child processes represented
 * by the AST nodes. It updates the shell's exit code based on the
 * termination status of each command.
 */
void	commands_wait(t_shell *ms, t_ast *ast, t_ast *limit)
{
	int	tmp;
	int	exit_type;

	if (ast == limit)
		return ;
	if (ast->type == T_CMND_AST)
	{
		tmp = pid_wait(ast, &exit_type);
		if (tmp >= 0)
			ms->exit_code = tmp;
		printf("Command '%s' exited with code %d\n", ast->value, ms->exit_code);
	}
	if (ast->left)
		commands_wait(ms, ast->left, limit);
	if (ast->right)
		commands_wait(ms, ast->right, limit);
}

/**
 * commands_can_continue - Checks if the command execution can continue.
 * @ms: A pointer to the shell structure containing environment variables.
 * @root: The root AST node of the commands.
 * @limit: The AST node representing the limit for waiting.
 *
 * This function waits for the commands in the AST and can be used
 * to determine if further command execution can proceed. It returns
 * 1 if execution can continue.
 */
int	commands_can_continue(t_shell *ms, t_ast *root, t_ast *limit)
{
	//i assume  we will need some more error checks
	commands_wait(ms, root, limit);
	return (1);
}

/**
 * command_id - Determines the command's ID based on its position in the pipeline.
 * @cmd: The current command AST node.
 * @prev: The previous command AST node.
 * @reset: A flag to reset the command ID tracking.
 *
 * This function returns an identifier indicating the command's position in a
 * potential pipeline. It differentiates between the first, last, and middle
 * commands in a pipeline.
 */
int command_id(t_ast *cmd, t_ast *prev, int reset)
{
    static int entry;

    if (reset)
    {
        entry = 0;
        return (-1);
    }
    
    // Adjust this condition to recognize pipeline context
    if (prev->type == T_PIPE_AST) // Only handle if it’s part of a pipeline
    {
        if (!entry) // First command in the pipeline
        {
            entry = 1;
            return (CMD_FIRST);
        }
        if (prev->right == cmd && !cmd->left && !cmd->right) // Last command in the pipeline
        {
            entry = 0;
            return (CMD_LAST);
        }
        return (CMD_MIDDLE); // Middle command in the pipeline
    }
    return (CMD_NOPIPE); // Not part of a pipeline
}

