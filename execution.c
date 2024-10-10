#include "../includes/minishell.h"

//////////

void print_pids(t_shell *ms)
{
    int i;

    if (!ms->pids || ms->pids->size == 0)
    {
        fprintf(stderr, "No PIDs to display.\n");
        return;
    }

    fprintf(stderr,"Collected PIDs:\n");
    for (i = 0; i < ms->pids->size; i++)
    {
        fprintf(stderr,"PID[%d]: %d\n", i, ms->pids->pids[i]);
    }
}

////////



int	ft_get_exit_status(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

//this does redirections to execute a command in the pipeline. 
void	exec_pipe_child(t_ast *node, t_shell *ms, char direction)
{
	int	status;

	if (direction == 'l')
	{
		close(ms->pipefd[0]); // Close read end
		if (dup2(ms->pipefd[1], STDOUT_FILENO) == -1)
			perror("dup2 fails");
		close(ms->pipefd[1]); // Close write end after duplication//???

	}
	else if (direction == 'r')
	{
		close(ms->pipefd[1]);
		if (dup2(ms->pipefd[0], STDIN_FILENO) == -1)
			perror("dup2 fails");
		close(ms->pipefd[0]);
	}
	status = ft_exec_node(ms, node, true);
	exit(status);
}

//thi sone is to handle one pipe
void	exec_pipeline(t_ast *ast, t_shell *ms)
{
	int	pid_l;
	int	pid_r;
	int	status;


	if (pipe(ms->pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid_l = fork();
	if (!pid_l) //child for l cmd
		exec_pipe_child(ast->left, ms, 'l');
	else
	{
		pid_r = fork();
		if (!pid_r) //child for r cmd
			exec_pipe_child(ast->right, ms, 'r');
		else //parent process
		{
			close(ms->pipefd[0]);
			close(ms->pipefd[1]);
			waitpid(pid_l, &status, 0);
			waitpid(pid_r, &status, 0);
			return ;
		}
	}
}

int	ft_exec_node(t_shell *ms, t_ast *node, bool piped)
{
	int	cmd_pid;

	if (!node) // Base case: No more commands to execute
		return (0);
	if (node->type == T_PIPE)
		exec_pipeline(node, ms);
	else
	{
		// If it's a single command, simply execute it
		cmd_pid = fork();
		if (cmd_pid == 0)
		{
			// Child process for single command
			child_process(ms, node); // Execute command
			exit(0); // Exit child process after execution
		}
		waitpid(cmd_pid, NULL, 0); // Wait for command to finish
		return (0);
	}
	return (0);// Recursive call for the right node
}
