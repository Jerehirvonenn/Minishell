#include "parsing.h"

t_ast	*create_ast_node(t_tokens *token)
{
	t_ast	*new_node;

	new_node = (t_ast *)malloc(sizeof(t_ast));
	if (!new_node)
		return (NULL); //handle memory error
	new_node->type = token->type; //need to get right type WIP
	new_node->value = line;
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

	while (args && args[len])
		len++;
	new_args = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_args)
		return (NULL); //WIP ERROR HANDLE
	i = 0;
	while (i < len)
		new_args[i++] = args[i];
	new_args[i++] = ft_strdup(to_add);
	new_args[i] = NULL;
	free(args);
	return (new_args);
}

t_ast	*parse_command(t_token **tokens)
{
	t_ast *node;

	if (!(*tokens))
		return (NULL);
	node = create_ast_node();
	if (!node)
		retunr (NULL);
	*tokens = (*tokens)->next;
	while (*tokens && (*tokens)->type == T_CMND)
	{
		node->exp_value = append_args(); //prog
		*tokens = (*tokens)->next;
	}
	return (node);
}

t_ast	*parsing_ast(t_token *tokens)
{
	t_ast *left;
	t_ast *right;

	left = parse_command(&tokens);
	while (*tokens && (*tokens)->type == T_PIPE)
	{
		*tokens = (*tokens)->next;
		//create pipe and assing commands
		//make pipe node the new left
	}
	return (left);
}
