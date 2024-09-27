#include "../includes/minishell.h"

t_ast	*create_ast_node(t_token_type type, char *str)
{
	t_ast	*new_node;

	new_node = (t_ast *)malloc(sizeof(t_ast));
	if (!new_node)
		return (NULL); //handle memory error
	new_node->type = type; //need to get right type WIP
	new_node->value = str;
	new_node->exp_value = NULL;
	new_node->io_list = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
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

//free toke nafter use

t_ast	*parse_command(t_token **tokens)
{
	t_ast *node;

	if (!(*tokens))
		return (NULL);
	node = create_ast_node((*tokens)->type, (*tokens)->value);
	if (!node)
		return (NULL);
	node->exp_value = append_args(node->exp_value, (*tokens)->value);
	*tokens = (*tokens)->next;
	while (*tokens && (*tokens)->type == T_CMND)
	{
		node->exp_value = append_args(node->exp_value, (*tokens)->value); //prog
		*tokens = (*tokens)->next;
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
