#include "../includes/minishell.h"

// Input redirection >
int	ft_in(t_io *io_list)
{
	int	fd;

	fd = open(io_list->value, O_RDONLY);
	if (fd == -1)
	{
		//error handling
		perror("open input");
        return (-1);
	}
	if(dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 input");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);  // Success
}

int	ft_out(t_io *io_list)
{
	int	fd;

	fd = open(io_list->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open output");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 output");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	ft_append(t_io *io_list)
{
	int	fd;

	fd = open(io_list->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open append");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 append");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}




//we use statuses here to check if it fails or not because this functions are meant
// to return 0 or -1
int	redirection(t_io *io_list)
{
	t_io	*current_io = io_list;
	int		status = 0;

	while (current_io)
	{
		if (current_io->type == T_IN)  // Input redirection
			status = ft_in(current_io);
		else if (current_io->type == T_OUT)  // Output redirection (overwrite)
			status = ft_out(current_io);
		else if (current_io->type == T_APPEND)  // Output redirection (append)
			status = ft_append(current_io);
		//else if (current_io->type == T_HEREDOC) // Heredoc redirection
			//status = ft_heredoc(current_io);

		if (status == -1)  // If any redirection fails, return error
			return (1);

		current_io = current_io->next;  // Move to the next redirection
	}

	return (0);  // Success
}

