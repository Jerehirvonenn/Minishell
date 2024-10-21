#include "../includes/minishell.h"

/*Step by step
 * 1. Generate a filename by using random bits from /dev/urandom
 * 2.create a temptorary file with the random name to /tmp/ms_$NAME
 * 3. open writing and reading fd for the created file and use
 * unlink to make sure it it destroyed after closing all existing fds of it
 *
 * 4.check if the heredoc is actually used? /if there is anotherredirectin after
 * 	4.A if so normal read
 * 	4.B if not file is not needed and heredoc info is not actully saved anywhere
 * 5.read with readline check for termianting word and EOF and ctrl+c
 * 6.check if the line needs to be expanded or nor based on quotes in the termianting word
 * 7.
 * /
 *
 * bash: syntax error near unexpected token `>>' for missing redirections
 */

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	ft_heredoc_getline(char *delim, int fd_write)
{
	char	*line;

	//set signals for heredoc!!!
	//check delimiter for expanding or not. both ' and " block expaning
	while (1)
	{
		line = readline(">");
		if (!ft_strcmp(line ,delim) || !line) //need to check for signal abort WIP, if delim is found or ctrl+c
			break;
		write(fd_write, line, ft_strlen(line));   //need to check expanding
		write(fd_write, "\n", 1);		//also func with check for write errors
		free(line);
	}
	free(line);
	//set signals back to normal
	return(0);
}

int	create_random_name(char *name, char *temp)
{
	int		fd_urandom;
	int		read_bits;
	char		*base;
	unsigned char	bit;

	fd_urandom = open("/dev/urandom", O_RDONLY);
	if (fd_urandom == -1)
		return (1);
	read_bits = read(fd_urandom, temp, 20);
	close(fd_urandom);
	if (read_bits < 0)
		return (1);
	base = "abcdefghijklmnopqrstuvwxyz0123456789";
	while (read_bits--)
	{
		bit = *temp;
		*name = base[bit % ft_strlen(base)];
		temp++;
		name++;
	}
	*name = '\0';
	return (0);
}

int	ft_createfile(int *fd_write, int *fd_read)
{
	char	filename[64]; //if you change size, rememebr to change in other funcs also!
	char	gen_name[21]; //rand namepart will be 20 chars so 20+null
	char	temp[20];

	ft_strlcpy(filename, "/tmp/ms_", 64);
	if (create_random_name(gen_name, temp))
	{
		//error_print("Error creating heredoc tmp file!\n", NULL);
		printf("Error with random name\n");
		return(1);
	}
	ft_strlcat(filename, gen_name, 64);
	*fd_write = open(filename, O_CREAT | O_EXCL | O_WRONLY | O_TRUNC, 0600);
	*fd_read = open(filename, O_RDONLY, 0600);
	if (*fd_write == -1 || *fd_read == -1)
	{
		//error_print("Error creating heredoc tmp file!\n", NULL);
		printf("Error opening\n");
		if (*fd_write != -1)  // Only close if fd_write is valid
			close(*fd_write);
		if (*fd_read != -1)  // Only close if fd_read is valid
			close(*fd_read);
		return (1);
	}
	//unlink(filename);
	return(0);
}

//takes t_io heredo node and adds the read fd to it.
int	ft_heredoc(t_ms *ms, t_io *io)
{
	int	fd_read;
	int	fd_write;
	//create random file name to /tmp/ms_{NAME}
	if (ft_createfile(&fd_write, &fd_read))
	{
		ms->stop = 1;
		return(1);
	}
	ft_heredoc_getline(io->value, fd_write); //change delim to actual delim
	close(fd_write);
	io->heredoc_fd = fd_read;
	return (0);
}

int	ast_heredoc(t_ast *root, t_ms *ms)
{
	t_io *io_temp;

	if (ms->stop || !root)
		return (1);
	io_temp = root->io_list;
	while (!ms->stop && root->type == T_CMND && io_temp)
	{
		if (io_temp->type == T_HEREDOC)  //create function that actually checks that the heredoc is used and uses empty one
			ft_heredoc(ms, io_temp);
		else
			io_temp = io_temp->next;
	}
	if (root->left)
		ast_heredoc(root->left, ms);
	if (root->right)
		ast_heredoc(root->right, ms);
	return (0);
}
/*
int main(void)
{
	return 0;
	//ft_heredoc();
}*/
