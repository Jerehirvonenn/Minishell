#include "../includes/minishell.h"

void		print_ast_tree(t_ast *root);
const char	*token_type_to_str(t_token_type type);
t_token	*ft_tokenize(char *str);

int main(int ac, char **av, char **envp)
{
	char prompt[1000] = "minishell> ";
	char *str;
	t_token *lst;
	t_token *temp_print;
	t_ast	*tree;

	(void)ac;
	(void)av;
	(void)envp;
	str = readline(prompt);
	if (!str)
		return (1);
	lst = ft_tokenize(str);

	temp_print = lst;
	while(temp_print)  //testing and printing out the tokens
	{
		printf("Token type %s\n", token_type_to_str(temp_print->type));
		temp_print = temp_print->next;
	}
	//creating the ast tree;
	tree = parsing_ast(lst);
	print_ast_tree(tree);
}
