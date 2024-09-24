#include "token.h"

int ft_isspace(char c)
{
    if (c == ' ' || c == '\t' || c == '\n'
	|| c == '\v' || c == '\f' || c == '\r')
        return 1;
    return 0;
}

int ft_isoperator(char c)
{
    if (c == '|' || c == '<' || c == '>')
        return 1;
    return 0;
}

enum e_token_type	ft_operator_type(char c)
{
	if (c == '|')
		return (T_PIPE);
	else if (c == '<')
		return (T_IN_REDIR);
	else if (c == '>')
		return (T_OUT_REDIR);
	else
		return (0);
}

char *ft_strndup(const char *src, int i)
{
    char	*dest;
    int		j;

    dest = (char *)malloc(sizeof(char) * (i + 1));
    if (!dest)
        return NULL;
    j = -1;
    while (++j < i)
        dest[j] = src[j];
    dest[i] = '\0';
    return dest;
}


void	create_operator_token(char **str, t_token **lst, t_token_type type)
{
	t_token *node;

	node = create_token(type, NULL);//create new node and add it to the back of the list
	if (!node)
		return ; //error handling needed
	add_token_to_list(lst, node);
	(*str)++;
}

void	create_argument_token(char **str, t_token **lst, t_token_type type)
{
	int 	i;
	char	*line;
	t_token *node;

	i = 0;
	while ((*str)[i] && !ft_isoperator((*str)[i]) && !ft_isspace((*str)[i]))
		i++;
	line = ft_strndup(*str, i);
	if (!line)
		return ; //error handling needed
	node = create_token(type, line);//create new node and add it to the back of the list
	if (!node)
		return ; //error handling needed
	add_token_to_list(lst, node);
	(*str) += i;
}

t_token	*ft_tokenize(char *str)
{
	t_token *lst;

	lst = NULL;
	while (*str)
	{
		while (ft_isspace(*str))
			str++;
		if (ft_isoperator(*str))
			create_operator_token(&str, &lst, ft_operator_type(*str));
		else
			create_argument_token(&str, &lst, T_CMND);
	}
	return (lst);
}

int main(int ac, char **av, char **envp)
{
	char prompt[1000] = "minishell> ";
	char *str;
	t_token *lst;
	t_ast	*tree;

	str = readline(prompt);
	lst = ft_tokenize(str);

	while(lst)
	{
		printf("Token type %i\n", lst->type);
		lst = lst->next;
	}

	//creating the ast tree;
	tree = parsing_ast();
}
