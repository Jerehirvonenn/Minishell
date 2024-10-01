#include "../includes/minishell.h"

int redirect(t_shell *ms, t_ast *ast, int cmd_id, int *prev_fd);
void minishell_close(int *fd);

/**
 * redirect - Handles input and output redirection for commands.
 * @ms: A pointer to the shell structure containing environment variables.
 * @ast: The AST node representing the command to execute.
 * @cmd_id: The command ID for the execution.
 * @prev_fd: An array containing the previous command's file descriptors.
 *
 * This function performs the necessary file descriptor manipulations
 * to redirect the standard input and output for commands in a pipeline.
 * It returns 1 on success or 0 on failure.
 */
int	redirect(t_shell *ms, t_ast *ast, int cmd_id, int *prev_fd)
{
	int	ret;

	if (cmd_id == CMD_NOPIPE) // No redirection needed if there's no pipe
		return (1);
	ret = 1;
	if (cmd_id > CMD_FIRST)
	{
		printf("Redirected stdin for '%s' from previous command (fd: %d)\n", ast->value, prev_fd[0]);
		// Redirect stdin from the previous command's pipe (read end)
		if (dup2(prev_fd[0], STDIN_FILENO) == -1)
		{
		    perror("dup2 (stdin)"); // Report any error
		    ret = 0; // Indicate failure
		}
	}
	if (cmd_id < CMD_LAST) // Redirect stdout to pipe's write end
	{
		printf("Redirecting stdout for '%s' to fd %d\n", ast->value, ms->pipe[1]);
		if (dup2(ms->pipe[1], STDOUT_FILENO) == -1)
		{
		    perror("dup2 (stdout)");
		    ret = 0;
		}
	}

	// Close the pipe in the child process after redirection is set
	minishell_close(ms->pipe);
	if (cmd_id < CMD_LAST && prev_fd[0] >= 0)
		close(prev_fd[0]);
	return (ret);
}

/**
 * minishell_close - Closes file descriptors safely.
 * @fd: An array of file descriptors to be closed.
 *
 * This function checks if the provided file descriptor array is valid
 * and if the descriptors are open. If they are, it closes them and
 * sets their values to -1 to indicate they are closed.
 */
void	minishell_close(int *fd)
{
	if (!fd)
		return ;
	if (fd[0] >= 0)
		close(fd[0]);
	if (fd[1] >= 0)
		close(fd[1]);
	fd[0] = -1;
	fd[1] = -1;
}
