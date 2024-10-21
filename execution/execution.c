#include "../includes/minishell.h"

int	count_commands(t_ast *node)
{
	int	count;

	count = 0;
	if (!node)
		return (0);
	if (node && node->type == T_CMND)
		count++;
	count += count_commands(node->left);
	count += count_commands(node->right);
	return (count);
}

void	wait_for_command(t_exec *exec)
{
	int	status;
	int	exit_status;
	int	signal_num;

	waitpid(exec->pids[exec->index], &status, 0);
	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		fprintf(stderr, "Process %d exited with status %d\n", exec->pids[exec->index], exit_status);
	}
	else if (WIFSIGNALED(status))
	{
		signal_num = WTERMSIG(status);
		fprintf(stderr, "Process %d was terminated by signal %d\n", exec->pids[exec->index], signal_num);
	}
}

void	wait_for_processes(t_exec *exec)
{
	int	j;
	int	status;
	int	exit_status;
	int	signal_num;

	j = 0;
	while (j <= exec->index)
	{
		waitpid(exec->pids[j], &status, 0);
		if (WIFEXITED(status))
		{
			exit_status = WEXITSTATUS(status);
			fprintf(stderr, "Process %d exited with status %d\n", exec->pids[j], exit_status);
		}
		else if (WIFSIGNALED(status))
		{
			signal_num = WTERMSIG(status);
			fprintf(stderr, "Process %d was terminated by signal %d\n", exec->pids[j], signal_num);
		}
		j++;
	}
}

void	execute_command(t_exec *exec, t_ast *node, t_ms *ms)
{
	bool	builtin;

	builtin = is_builtin(node);
	if (builtin)
		exec_builtin(ms, node, exec);
	else
	{
		exec->pids[exec->index] = fork();
		if (exec->pids[exec->index] == -1)
		{
			perror("Fork Error");
			exit(EXIT_FAILURE);
		}
		else if (exec->pids[exec->index] == 0)
		{
			if (redirection(node) != 0)
			{
				perror("redirection failed");
				exit(EXIT_FAILURE);
			}
			child_process(ms, node, exec);
			exit(EXIT_SUCCESS);
		}
		wait_for_command(exec);
		exec->index++;
	}
}

void	handle_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	fork_left(t_ast *node, t_exec *exec, t_ms *ms)
{
	close(exec->pipefd[0]);
	dup2(exec->pipefd[1], STDOUT_FILENO);
	close(exec->pipefd[1]);
	execute_ast(node->left, exec, ms);
	exit(EXIT_SUCCESS);
}

void	fork_right(t_ast *node, t_exec *exec, t_ms *ms)
{
	close(exec->pipefd[1]);
	dup2(exec->pipefd[0], STDIN_FILENO);
	close(exec->pipefd[0]);
	execute_ast(node->right, exec, ms);
	exit(EXIT_SUCCESS);
}

void	execute_pipe(t_ast *node, t_exec *exec, t_ms *ms)
{
	if (pipe(exec->pipefd) == -1)
		handle_error("Pipe failure");
	exec->pids[exec->index] = fork();
	if (exec->pids[exec->index] == -1)
		handle_error("Fork error");
	else if (exec->pids[exec->index] == 0)
		fork_left(node, exec, ms);
	exec->index++;
	exec->pids[exec->index] = fork();
	if (exec->pids[exec->index] == -1)
		handle_error("Fork error");
	else if (exec->pids[exec->index] == 0)
		fork_right(node, exec, ms);
	close(exec->pipefd[0]);
	close(exec->pipefd[1]);
	wait_for_processes(exec);
}

void	expand_ast(t_ast *node, t_exec *ms);
void print_ast_tree(t_ast *root);

void	execute_ast(t_ast *node, t_exec *exec, t_ms *ms)
{
	if (!node)
		return ;
	if (node->type == T_PIPE)
		execute_pipe(node, exec, ms);
	else if (node->type == T_CMND)
	{
		//expand_ast(node, ms);
		//print_ast_tree(node);
		execute_command(exec, node, ms);
	}
}
