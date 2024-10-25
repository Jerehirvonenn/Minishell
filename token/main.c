#include "../includes/minishell.h"

int ms_signal = 0;

void		print_ast_tree(t_ast *root);
const char	*token_type_to_str(t_token_type type);
t_token	*ft_tokenize(char *str, t_ms *ms);
void	print_tokens(t_token *tokens);

void	init_minishell(t_ms *ms, char **envp)
{
	ms->my_envp = envp;
	ms->exit_code = 0;
	ms->envp_size = 0;
	ms->stop = 0;
	ms->quit = 0;
	ms->pwd = getcwd(NULL, 0);
	ms->old_pwd = NULL;
	ms->ast = NULL;
	ms->tokens = NULL;
}

void	reset_ms(t_ms *ms)
{
	ms->stop = 0;
	ms->quit = 0;
	ms->ast = NULL;
	ms->tokens = NULL;
}

int	main(int ac, char **av, char **envp)
{
	char prompt[1000] = "minishell> ";
	char *str;
	t_ms	ms;

	(void)ac;
	(void)av;
	init_minishell(&ms, envp);
	signal_handler();
	while (1)
	{
		reset_ms(&ms);
		//reset what needs to be resetted for start
		str = readline(prompt);
		if (!str)
			return (0);
		if (!*str)
			continue;
		ms.tokens  = ft_tokenize(str, &ms);
		if (ms.stop)
			continue;
		print_tokens(ms.tokens); //DEBUG
		//creating the ast tree;
		ms.ast = parsing_ast(ms.tokens, &ms);
		if (ms.stop)
			continue;
		expand_ast(ms.ast, &ms);
		print_ast_tree(ms.ast);  //debug
		ast_heredoc(ms.ast, &ms);
		execute_ast(ms.ast, &ms);
		ft_free_ast(ms.ast);
		ft_free_token(ms.tokens);
	}
}
