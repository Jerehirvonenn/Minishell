#ifndef TOKEN_H
# define TOKEN_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

typedef enum e_token_type
{
	T_CMND,			//command names, filenames, and arguments.
	T_IN_REDIR,		//input redirection <
	T_OUT_REDIR,		//output redirection >
	T_HERE_DOC,		//Heredoc <<
	T_APEND,		//Append >>
	T_PIPE,			//pipe
}	t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char			*value;
	struct s_token		*next;
	struct s_token		*prev;
}	t_token;

//token_create.c
t_token	*create_token(t_token_type type, char *value);
void	ft_free_token(t_token *head);
void	add_token_to_list(t_token **lst, t_token *new_token);

#endif
