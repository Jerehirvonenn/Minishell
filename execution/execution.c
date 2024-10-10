#include "../includes/minishell.h"

int	count_commands(t_ast *node)
{
	int	count;

	count = 0;
	if (!node)
		return (0);
	// If the node is a command, count it
	if (node && node->type == T_CMND)
		count++;
	// Otherwise, traverse the left and right subtrees
	count += count_commands(node->left);
	count += count_commands(node->right);
	return (count);
}

/*void	execute_ast(t_ast *node, t_shell *ms)
{
	if (!node)
		return ;
	if (node->type == T_PIPE)
	{
		if (pipe(ms->pipefd) == -1)
		{
			perror("Pipe Error");
			exit(EXIT_FAILURE);
		}
		ms->pids[ms->index] = fork();
		if (ms->pids[ms->index] == -1)
		{
			perror("Fork Error");
			exit(EXIT_FAILURE);
		}
		else if (ms->pids[ms->index] == 0)
		{
			close(ms->pipefd[0]);
			dup2(ms->pipefd[1], STDOUT_FILENO);
			close(ms->pipefd[1]);
			execute_ast(node->left, ms);
			exit(EXIT_SUCCESS);
		}
		ms->index++;
		ms->pids[ms->index] = fork();
		if (ms->pids[ms->index] == -1)
		{
			perror("Fork Error");
			exit(EXIT_FAILURE);
		}
		else if (ms->pids[ms->index] == 0)
		{
			close(ms->pipefd[1]);
			dup2(ms->pipefd[0], STDIN_FILENO);
			close(ms->pipefd[0]); 
			execute_ast(node->right, ms);
			exit(EXIT_SUCCESS);
		}
		close(ms->pipefd[0]);
		close(ms->pipefd[1]);
		int j = 0;
		while (j <= ms->index)
		{
			int status;
			waitpid(ms->pids[j], &status, 0);
			if (WIFEXITED(status))
			{
				int exit_status = WEXITSTATUS(status);
				fprintf(stderr, "Process %d exited with status %d\n", ms->pids[j], exit_status);
			}
			else if (WIFSIGNALED(status))
			{
				int signal_num = WTERMSIG(status);
				fprintf(stderr, "Process %d was terminated by signal %d\n", ms->pids[j], signal_num);
			}
		}
	}
	else if (node->type == T_CMND)
	{
		ms->pids[ms->index] = fork();
		if (ms->pids[ms->index] == -1)
		{
			perror("Fork Error");
			exit(EXIT_FAILURE);
		}
		else if (ms->pids[ms->index] == 0)
		{
			child_process(ms, node);
			exit(EXIT_SUCCESS);
		}
		int status;
		waitpid(ms->pids[ms->index], &status, 0);
		if (WIFEXITED(status))
		{
			int exit_status = WEXITSTATUS(status);
			fprintf(stderr, "Process %d exited with status %d\n", ms->pids[ms->index], exit_status);
		}
		else if (WIFSIGNALED(status))
		{
			int signal_num = WTERMSIG(status);
			fprintf(stderr, "Process %d was terminated by signal %d\n", ms->pids[ms->index], signal_num);
		}
		ms->index++;
	}
}*/

void	execute_ast(t_ast *node, t_shell *ms);
void	wait_for_command(t_shell *ms)
{
	int status;
	int exit_status;
	int signal_num;

    waitpid(ms->pids[ms->index], &status, 0);
    if (WIFEXITED(status))
    {
        exit_status = WEXITSTATUS(status);
        //fprintf(stderr, "Process %d exited with status %d\n", ms->pids[ms->index], exit_status);
    }
    else if (WIFSIGNALED(status))
    {
        signal_num = WTERMSIG(status);
        //fprintf(stderr, "Process %d was terminated by signal %d\n", ms->pids[ms->index], signal_num);
    }
}
void	wait_for_processes(t_shell *ms)
{
	int	j;
	int	status;
	int	exit_status;
	int signal_num;

	j = 0;
    while  (j <= ms->index)
    {
        waitpid(ms->pids[j], &status, 0);
        if (WIFEXITED(status))
        {
            exit_status = WEXITSTATUS(status);
            //fprintf(stderr, "Process %d exited with status %d\n", ms->pids[j], exit_status);
        }
        else if (WIFSIGNALED(status))
        {
            signal_num = WTERMSIG(status);
            //fprintf(stderr, "Process %d was terminated by signal %d\n", ms->pids[j], signal_num);
        }
		j++;
    }
}

int	exec_buildin(t_shell *ms, t_ast *ast);

void	execute_command(t_shell *ms, t_ast *node)
{
	fprintf(stderr, "we are here3\n");
	if (node->value == NULL)
		fprintf(stderr, "node is null");
	bool buildin = is_buildin(node);
	if (buildin)
		exec_buildin(ms, node);
	else
	{
		ms->pids[ms->index] = fork();
		if (ms->pids[ms->index] == -1)
		{
			perror("Fork Error");
			exit(EXIT_FAILURE);
		}
		else if (ms->pids[ms->index] == 0)
		{
			child_process(ms, node);
			exit(EXIT_SUCCESS); // Exit the child process after execution
		}
		wait_for_command(ms);
		ms->index++;
	}
}

void	execute_pipe(t_ast *node, t_shell *ms)
{
    if (pipe(ms->pipefd) == -1)
    {
        perror("Pipe Error");
        exit(EXIT_FAILURE);
    }
	ms->pids[ms->index] = fork();
    if (ms->pids[ms->index] == -1)
    {
        perror("Fork Error");
        exit(EXIT_FAILURE);
    }
    else if (ms->pids[ms->index] == 0)
    {
        close(ms->pipefd[0]);
        dup2(ms->pipefd[1], STDOUT_FILENO);
        close(ms->pipefd[1]);
        execute_ast(node->left, ms);
        exit(EXIT_SUCCESS);
    }
	ms->index++;
	ms->pids[ms->index] = fork();
	if (ms->pids[ms->index] == -1)
    {
        perror("Fork Error");
        exit(EXIT_FAILURE);
    }
    else if (ms->pids[ms->index] == 0)
    {
        close(ms->pipefd[1]);
        dup2(ms->pipefd[0], STDIN_FILENO);
        close(ms->pipefd[0]);
        execute_ast(node->right, ms);
        exit(EXIT_SUCCESS);
	}
    close(ms->pipefd[0]);
    close(ms->pipefd[1]);
    wait_for_processes(ms);
}

void	execute_ast(t_ast *node, t_shell *ms)
{
	if (!node)
		return ;
	if (node->type == T_PIPE)
		execute_pipe(node, ms);
	else if (node->type == T_CMND)
	{
		fprintf(stderr, "we are here2\n");
		execute_command(ms, node);
	}
}
