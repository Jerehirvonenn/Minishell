#include "../includes/minishell.h"

void		print_ast_tree(t_ast *root);
const char	*token_type_to_str(t_token_type type);
t_token	*ft_tokenize(char *str);
void	print_tokens(t_token *tokens);

void	init_minishell(t_ms *ms_data, char **envp)
{
	ms_data->my_envp = envp;
	ms_data->exit_code = 0;
	ms_data->envp_size = 0;
	ms_data->ms_stop = 0;
	ms_data->ms_quit = 0;
	ms_data->pwd = getcwd(NULL, 0);
	ms_data->old_pwd = NULL;
	ms_data->ast = NULL;
}

int	main(int ac, char **av, char **envp)
{
	char prompt[1000] = "minishell> ";
	char *str;
	t_token *lst;
	t_ast	*tree;
	t_ms	ms_data;

	(void)ac;
	(void)av;
	init_minishell(&ms_data);
	signal_handler();
	while (1)
	{
		str = readline(prompt);
		if (!str)
			return (1);
		if (!*str)
			continue;
		lst = ft_tokenize(str);
		print_tokens(lst);  //debug
		//creating the ast tree;
		tree = parsing_ast(lst);
		print_ast_tree(tree);  //debug
		ft_free_ast(tree);
		ft_free_token(lst);
		free(str);
	}
}
