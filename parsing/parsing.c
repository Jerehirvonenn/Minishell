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

	//free value
	free(node->value);
	//free exp value
	i = 0;
	while (node->exp_value && node->exp_value[i])
	{
		free(node->exp_value[i]);
		i++;
	}
	free(node->exp_value);
	//free I/O list
	ft_free_io_list(node->io_list);
	//free the ast node
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
		new_node->value = ft_strdup(str);
	else
		new_node->value = NULL;;
	new_node->exp_value = NULL;
	new_node->io_list = NULL;
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

char	**append_args(char **args, char *to_add)
{
	int len;
	int i;
	char **new_args;

	len = 0;
	while (args && args[len])
		len++;
	new_args = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_args)
		return (NULL); //WIP ERROR HANDLE
	i = -1;
	while (++i < len)
		new_args[i] = args[i];
	new_args[i++] = ft_strdup(to_add);
	new_args[i] = NULL;
	free(args);
	return (new_args);
}


// Function to create a new I/O node
t_io	*create_io_node(t_io_type type, char *value)
{
	t_io	*new_io;

	new_io = (t_io *)malloc(sizeof(t_io));
	if (!new_io)
		return (NULL); // handle memory error
	new_io->type = type;
	new_io->value = ft_strdup(value); // Duplicate the value
	new_io->next = NULL;
	new_io->prev = NULL;
	return (new_io);
}

// Function to append an I/O node to the AST node's I/O list
void	add_io_to_ast(t_ast *ast_node, t_io_type io_type, char *io_value)
{
	t_io	*new_io;
	t_io	*last;

	if (!ast_node)
		return; // safety check

	// Create a new I/O node
	new_io = create_io_node(io_type, io_value);
	if (!new_io)
		return; // handle memory error

	// If the AST node has no I/O redirections yet, set the new I/O node as the first one
	if (!ast_node->io_list)
	{
		ast_node->io_list = new_io;
	}
	else
	{
		// Traverse to the end of the I/O list and append the new I/O node
		last = ast_node->io_list;
		while (last->next)
			last = last->next;
		last->next = new_io;
		new_io->prev = last;
	}
}
//free token after use

/*t_ast	*parse_command_reverse(t_token **token)
{
	t_ast *node;

	node = create_ast_node(T_CMND, NULL);
	if (!node)
		return (NULL); //malloc failure
	if ((*token)->type = T_PIPE)
		exit (1);  //Parsing error WIP
	while (ft_isredirection((*token)->type))
		add_io(); //WIP
	if ((*token)->type = T_CMND)
		node->value = (*token)->value;
	return (node);
}*/
/*
t_ast	*parse_command(t_token **tokens)
{
	t_ast *node;

	if (!(*tokens))
		return (NULL);
	//if ((*tokens)->type != T_CMND)
	//	return (parse_command_reverese();) //make it work if it starts with io, check if its valid also
	node = create_ast_node(T_CMND, (*tokens)->value);
	if (!node)
		return (NULL);
	node->exp_value = append_args(node->exp_value, (*tokens)->value);
	*tokens = (*tokens)->next;
	while (*tokens && ((*tokens)->type == T_CMND || ft_isredirection((*tokens)->type))) //add workaround for also adding io
	{
		if ((*tokens)->type == T_CMND)
		{
			node->exp_value = append_args(node->exp_value, (*tokens)->value); //prog
			*tokens = (*tokens)->next;
		}
		else if(ft_isredirection((*tokens)->type) && (*tokens)->next->type ==T_CMND)
		{
			add_io_to_ast(node, token_to_io_type((*tokens)->type), (*tokens)->next->value); //WIP
			*tokens = (*tokens)->next;
			*tokens = (*tokens)->next;
		}
		else
			exit (1); //parse error
	}
	return (node);
}*/
//tried to make 1 function that works on all ordres of cmnds and redirections. need more testing
t_ast	*parse_command(t_token **tokens)
{
	t_ast *node;

	if (!(*tokens))
		return (NULL);
	if ((*tokens)->type == T_CMND)
		node = create_ast_node(T_CMND, (*tokens)->value);
	else
		node = create_ast_node(T_CMND, NULL);
	if (!node)
		return (NULL); //MALLOC FAILURE
	if (node->value)
	{
		node->exp_value = append_args(node->exp_value, (*tokens)->value);
		*tokens = (*tokens)->next;
	}
	while (*tokens && ((*tokens)->type == T_CMND || ft_isredirection((*tokens)->type)))
	{
		if ((*tokens)->type == T_CMND)
		{
			if (!node->value)
			{
				node->value = (*tokens)->value;
				node->exp_value = append_args(node->exp_value, (*tokens)->value);
			}
			else
				node->exp_value = append_args(node->exp_value, (*tokens)->value); //prog
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
			printf("No target for redirection\n");
			exit(1);
		}
	}
	return (node);
}

t_ast	*parsing_ast(t_token *tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe;

	left = parse_command(&tokens);
	while (tokens && tokens->type == T_PIPE)
	{
		tokens = tokens->next;
		if (tokens->type == T_PIPE)
		{
			printf("Parsing error\n");  //how to handle like bash?
			exit(1);
		}
		right = parse_command(&tokens);
		//create pipe and assing commands
		pipe = create_ast_node(T_PIPE, NULL);
		//make pipe node the new left
		pipe->left = left;
		pipe->right = right;
		left = pipe;
	}
	return (left);
}
