#include "token.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token *token = malloc(sizeof(t_token));
	token->type = type;
	token->value = value;
	token->next = NULL;
	token->prev = NULL;
	return token;
}

void	add_token_to_list(t_token **lst, t_token *new_token)
{
	t_token *temp;

	if (!*lst)
		*lst = new_token;
	else 
	{
		temp = *lst;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_token;
		new_token->prev = temp;
	}
}

