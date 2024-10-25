#include "../includes/minishell.h"

void	ft_free_io_list(t_io *io_list)
{
	t_io	*temp;

	while (io_list)
	{
		free(io_list->value);
		temp = io_list;
		io_list = io_list->next;
		free(temp);
	}
}

void	ft_free_ast_node(t_ast *node)
{
	int	i;

	free(node->value);
	i = 0;
	while (node->exp_value && node->exp_value[i])
	{
		free(node->exp_value[i]);
		i++;
	}
	free(node->exp_value);
	ft_free_io_list(node->io_list);
	free(node);
}

void	ft_free_ast(t_ast *root)
{
	if (root->left)
		ft_free_ast(root->left);
	if (root->right)
		ft_free_ast(root->right);
	ft_free_ast_node(root);
}

int	ft_isredirection(enum e_token_type type)
{
	return (type == T_IN_REDIR || type == T_OUT_REDIR || type == T_HERE_DOC || type == T_APEND);
	exit(1);
}

t_ast	*create_ast_node(t_token_type type, char *str)
{
	t_ast	*new_node;

	new_node = (t_ast *)malloc(sizeof(t_ast));
	if (!new_node)
		return (NULL); //handle memory error
	new_node->type = type;
	if (str)
	{
		new_node->value = ft_strdup(str);
		if (!new_node->value)
		{
			//ms->stop = 1;
			free(new_node);
			return (NULL);
		}
	}
	else
		new_node->value = NULL;;
	new_node->exp_value = NULL;
	new_node->io_list = NULL;
	new_node->pid = -1;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_io_type token_to_io_type(t_token_type type)
{
	if (type == T_IN_REDIR)
		return (T_IN);
	else if (type == T_OUT_REDIR)
		return (T_OUT);
	else if (type == T_HERE_DOC)
		return (T_HEREDOC);
	else
		return (T_APPEND);
}

char	**append_args(char **args, char *to_add, t_ms *ms)
{
	int len;
	int i;
	char **new_args;

	len = 0;
	i = -1;
	while (args && args[len])
		len++;
	new_args = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_args)
	{
		free_array(args);
		ms->stop = 1;
		return (NULL); //WIP ERROR HANDLE
	}
	while (++i < len)
		new_args[i] = args[i];
	new_args[i] = ft_strdup(to_add);
	if (!new_args[i])
	{
		free_array(args);
		ms->stop = 1;
		return (NULL);
	}
	new_args[++i] = NULL;
	free(args);
	return (new_args);
}


// Function to create a new I/O node
t_io	*create_io_node(t_io_type type, char *value)
{
	t_io	*new_io;

	new_io = (t_io *)malloc(sizeof(t_io));
	if (!new_io)
		return (NULL);
	new_io->type = type;
	new_io->value = ft_strdup(value); // Duplicate the value
	if (!new_io->value)
	{
		free(new_io);
		return (NULL);
	}
	new_io->heredoc_fd = -1;
	new_io->next = NULL;
	new_io->prev = NULL;
	return (new_io);
}

// Function to append an I/O node to the AST node's I/O list
int	add_io_to_ast(t_ast *ast_node, t_io_type io_type, char *io_value)
{
	t_io	*new_io;
	t_io	*last;

	if (!ast_node)
		return (1);
	// Create a new I/O node
	new_io = create_io_node(io_type, io_value);
	if (!new_io)
	{
		ft_free_ast_node(ast_node);
		return (1); // handle memory error
	}
	// If the AST node has no I/O redirections yet, set the new I/O node as the first one
	if (!ast_node->io_list)
		ast_node->io_list = new_io;
	else
	{
		// Traverse to the end of the I/O list and append the new I/O node
		last = ast_node->io_list;
		while (last->next)
			last = last->next;
		last->next = new_io;
		new_io->prev = last;
	}
	return (0);
}

//tried to make 1 function that works on all ordres of cmnds and redirections. need more testing
t_ast	*parse_command(t_token **tokens, t_ms *ms)
{
	t_ast *node;

	if ((*tokens)->type == T_CMND)
		node = create_ast_node(T_CMND, (*tokens)->value);
	else
		node = create_ast_node(T_CMND, NULL);
	if (!node)
	{
		printf("minishell: cannot allocate memory");
		ms->quit = 1;
		return (NULL); //MALLOC FAILURE
	}
	if (node->value)
	{
		node->exp_value = append_args(node->exp_value, (*tokens)->value, ms);
		*tokens = (*tokens)->next;
	}
	while (*tokens && ((*tokens)->type == T_CMND || ft_isredirection((*tokens)->type)))
	{
		if ((*tokens)->type == T_CMND)
		{
			if (!node->value)
				node->value = (*tokens)->value;
			node->exp_value = append_args(node->exp_value, (*tokens)->value, ms); //prog
			*tokens = (*tokens)->next;
		}
		else if(ft_isredirection((*tokens)->type) && (*tokens)->next && (*tokens)->next->type ==T_CMND)
		{
			add_io_to_ast(node, token_to_io_type((*tokens)->type), (*tokens)->next->value); //WIP
			*tokens = (*tokens)->next;
			*tokens = (*tokens)->next;
		}
		else
		{
			printf("Syntax error\n");
			ms->stop = 1;
			ft_free_ast_node(node);
			return (NULL);  //need to free current node
		}
	}
	return (node);
}

t_ast	*parsing_ast(t_token *tokens, t_ms *ms)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe;

	if (!tokens || tokens->type == T_PIPE)
	{
		printf("parsing error\n"); //change and free tokens
		ms->stop = 1;
		return (NULL);
	}
	left = parse_command(&tokens, ms);
	/*if (!left)
	{
		//stop or quit.
	}*/
	while (tokens && tokens->type == T_PIPE)
	{
		tokens = tokens->next;
		if (!tokens || tokens->type == T_PIPE)
		{
			printf("Parsing error\n");  //how to handle like bash?
			ms->stop = 1;
			return (NULL);
		}
		right = parse_command(&tokens, ms);
		//create pipe and assing commands
		pipe = create_ast_node(T_PIPE, NULL);
		//make pipe node the new left
		pipe->left = left;
		pipe->right = right;
		left = pipe;
	}
	return (left);
}
