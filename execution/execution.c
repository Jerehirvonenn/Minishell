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
void	print_array(t_ms *ms);
	
void	execute_middle_command(t_ast *node, t_ms *ms, int *pipefd, int *write_pipe)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("fork error");
		ft_free_ast(node);
		clean_ms(ms);
		ms->quit = 1;
		return ;
	}
	else if (node->pid == 0)
	{

		//close_fd(&pipefd[1]);//kinda useless
		//close_fd(&write_pipe[0]);
		if ((dup2(pipefd[0], 0) == -1) || (dup2(write_pipe[1], 1) == -1)) //DUP!!
		{
			perror("dup2 for pipe input failed");
			close_array_fds(ms);
			exit(EXIT_FAILURE);
		}
		//close_fd(&write_pipe[0]);
		//close_fd(&pipefd[0]);
		//close_fd(&write_pipe[1]);
		close_array_fds(ms);
		if (redirection(node) == -1) //REDIRECTION
			exit(EXIT_FAILURE);
		child_process(ms, node);
		exit(EXIT_SUCCESS);
	}
	//close_fd(&pipefd[0]);
	//close_fd(&write_pipe[1]);
	close_and_change_array(ms, &pipefd[0]);
	close_and_change_array(ms, &write_pipe[1]);
}

void	execute_last_command(t_ast *node, t_ms *ms, int *pipefd)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("fork error");
		ft_free_ast(node);
		clean_ms(ms);
		ms->quit = 1;
		return ;
	}
	else if (node->pid == 0)
	{
		if (dup2(pipefd[0], 0) == -1)//DUP!!
		{
			perror("dup2 last cmd fails");
			//ms->quit = 1;
			//return ;
			close_array_fds(ms);
			exit(EXIT_FAILURE);
		}
		//close_fd(&pipefd[0]);
		close_array_fds(ms);
		if (redirection(node) == 1)
			exit(EXIT_FAILURE);
		child_process(ms, node);
		exit(EXIT_SUCCESS);
	}
	//close_fd(&pipefd[0]);
	close_and_change_array(ms, &pipefd[0]);
}

void	execute_first_command(t_ast *node, t_ms *ms, int *pipefd)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("fork error");
		ft_free_ast(node);
		clean_ms(ms);
		ms->quit = 1;
		return ;
	}
	else if (node->pid == 0)
	{
		if (dup2(pipefd[1], 1) == -1) 
		{
			perror("dup2 first cmd fails"); //here it might be all opened pipes 
			close_array_fds(ms);//need to close everything
			exit(EXIT_FAILURE);
		}
		//close_fd(&pipefd[0]);
		//close_fd(&pipefd[1]);
		close_array_fds(ms);
		if (redirection(node) == -1) //REDIRECTION
			exit(EXIT_FAILURE);//??
		child_process(ms, node);
		exit(EXIT_SUCCESS);
	}
	//close_fd(&pipefd[1]);
	close_and_change_array(ms, &pipefd[1]);
}

void	execute_command(t_ast *node, t_ms *ms)
{
	node->pid = fork();
	if (node->pid == -1)
	{
		perror("fork error");
		ft_free_ast(node);
		clean_ms(ms);
		exit(EXIT_FAILURE);
	}
	else if (node->pid == 0) //child process
	{
		if (redirection(node) == -1)
			exit(EXIT_FAILURE);
		child_process(ms, node);
		exit(EXIT_SUCCESS);
	}
}

void	execute_pipe(t_ast *node, t_ms *ms, int *write_pipe)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe failure");
		ft_free_ast(node);
		clean_ms(ms);
		ms->quit = 1;
		return ;
	}
	add_to_array(ms, pipefd[0]);
	add_to_array(ms, pipefd[1]);
	if (node->left && node->left->type == T_PIPE)
		execute_pipe(node->left, ms, pipefd);
	else if (node->left && node->left->type == T_CMND)
		execute_first_command(node->left, ms, pipefd);
	if (node->right && node->right->type == T_CMND && ms->quit == 0)
		execute_middle_command(node->right, ms, pipefd, write_pipe);
	close_multiple_fds(pipefd);
	return ;
}

void	execute_first_pipe(t_ast *node, t_ms *ms)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe failure");
		ft_free_ast(node);
		clean_ms(ms);
		exit(EXIT_FAILURE);
	}
	add_to_array(ms, pipefd[0]);
	add_to_array(ms, pipefd[1]);
	if (node->left && node->left->type == T_PIPE)
		execute_pipe(node->left, ms, pipefd);
	else if (node->left && node->left->type == T_CMND)
		execute_first_command(node->left, ms, pipefd);
	if (node->right && node->right->type == T_CMND && ms->quit == 0)
		execute_last_command(node->right, ms, pipefd);
}

void	ft_waiting(t_ast *node)
{
	static int	status;

	if (node->type == T_CMND)
		waitpid(node->pid, &status, 0);
	if (node->left)
		ft_waiting(node->left);
	if (node->right)
		ft_waiting(node->right);
}


void	print_array(t_ms *ms)
{
	int	i;

	i = 0;
	while (i < 50)
	{
		fprintf(stderr, "A: %d\n", ms->fds[i]);
		i++;
	}
}

void	execute_ast(t_ast *node,  t_ms *ms)
{
	int	i;

	i = 0;
	while (i < 1024)
	{
		ms->fds[i] = -1;
		i++;
	}
	if (!node)
		return ;
	if (node->type == T_PIPE)
		execute_first_pipe(node, ms);
	else if (node->type == T_CMND)
		execute_command(node, ms);
	ft_waiting(node);
}
