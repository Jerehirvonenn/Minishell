#include "../includes/minishell.h"

/*
   1.create a pipe
   2.check if left is pipe or command
   3.if pipe we call execute pipe again and give it the pipe created.
   4.if cmnd call exec command function
   5.handle right part
   6.call exec command on right node
   ________
   1. fork using pid from a struct
   2. check the error and cleanup 
   3. inside child process print out the name of the cmd
   4. 
   _______
   execute first:
   1. fork
   2. inside child process i close read_fd
   3. write (pipe[1], line, ft_strlen(line))
   4. close wr_end
   5. parent closes wr_end
   _____
   execute middle:
   1. fork 
   2.inside child process read (pipefd[0])
   3.			close reading end
   4.			write (write_pipe[1]);
   5.			close writing end of write_pipe
   6.close reading and writing wnds in the parent 
   ______
   las cmd:
   1. fork
   2. inside child read (pipefd[0], buf, bufsize);
   3. in child and parent close rd_end
*/

void	execute_middle_command(t_ast *node, t_ms *ms, int *pipefd, int *write_pipe)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("fork error");
		//clean up everything
	}
	else if (node->pid == 0)
	{

		close(pipefd[1]);
		close(write_pipe[0]);
		dup2(pipefd[0], 0);
		dup2(write_pipe[1], 1);
		close(pipefd[0]);
		close(write_pipe[1]);
		if (redirection(node) == 0)
			child_process(ms, node);
		exit(1);
	}
	close(pipefd[0]);
	close(write_pipe[1]);


}

void	execute_last_command(t_ast *node, t_ms *ms, int *pipefd)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("fork error");
		//clean up everything
	}
	else if (node->pid == 0)
	{
		dup2(pipefd[0], 0);
		close(pipefd[0]);
		if (redirection(node) == 0)
			child_process(ms, node);
		exit(1);
	}
	close(pipefd[0]);

}

void	execute_first_command(t_ast *node, t_ms *ms, int *pipefd)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("fork error");
		//clean up everything
	}
	else if (node->pid == 0)
	{
		dup2(pipefd[1], 1);
		close(pipefd[0]);
		close(pipefd[1]);
		if (redirection(node) == 0)
			child_process(ms, node);
		exit(1);
	}
	close(pipefd[1]);

}

void	execute_command(t_ast *node, t_ms *ms)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("fork error");
		//clean up everything
	}
	else if (node->pid == 0) //child process
	{
		if (redirection(node) == 0)
		{
			fprintf(stderr, "heredoc\n");
			child_process(ms, node);
		}
		exit(1);
	}


}

void	execute_pipe(t_ast *node, t_ms *ms, int *write_pipe)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe failure");
		//clean everything
	}
	if (node->left && node->left->type == T_PIPE)
		execute_pipe(node->left, ms, pipefd);
	else if (node->left && node->left->type == T_CMND)
		execute_first_command(node->left, ms, pipefd);
	if (node->right && node->right->type == T_CMND)
		execute_middle_command(node->right, ms, pipefd, write_pipe);
}

void	execute_first_pipe(t_ast *node, t_ms *ms)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe failure");
		//clean everything
	}
	if (node->left && node->left->type == T_PIPE)
		execute_pipe(node->left, ms, pipefd);
	else if (node->left && node->left->type == T_CMND)
		execute_first_command(node->left, ms, pipefd);
	if (node->right && node->right->type == T_CMND)
		execute_last_command(node->right, ms, pipefd);
}

void	ft_waiting(t_ast *node)
{
	static int	status;

	if (node->type == T_CMND)
	//fprintf(stderr, "before waitpid\n");
		waitpid(node->pid, &status, 0);
	//fprintf(stderr, "after waitpid\n");
	if (node->left)
		ft_waiting(node->left);
	if (node->right)
		ft_waiting(node->right);
}

void	execute_ast(t_ast *node,  t_ms *ms)
{
	if (!node)
		return ;
	if (node->type == T_PIPE)
		execute_first_pipe(node, ms);
	else if (node->type == T_CMND)
		execute_command(node, ms);
	ft_waiting(node);
}
/*
   1. redirection before each child process func
   2. figure out how to handle here doc
   	2.1 heredoc doesnt have value for the file name
	2.2 fd for reading is stored in node->heredoc_fd
	2.3 if heredoc_fd is -1, heredoc is not actually used
   3. waitpid func for ast tree
   4. go through ast tree and expand everything but hdoc value
   5.be a good girl for daddy
*/
