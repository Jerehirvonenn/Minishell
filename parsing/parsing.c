#include "parsing.h"

t_ast	*parse_command(t_token **tokens)
{
	t_ast *node;

	if (!(*tokens))
		return (NULL);
	node = create_ast_node(); //prog
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
