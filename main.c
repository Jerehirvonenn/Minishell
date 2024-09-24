#include "minishell.h"

void	execute_buildin(t_shell *ms, char **av);
bool	special_buildin(int ac, char **av);

int	main(int ac, char **av)
{
	t_shell ms;
	t_token *tokens;
	t_ast	*ast;
	char	*input;
	int status = 0;

	init_shell(&ms);
	//this is for cases of special buildins
	/*if (special_buildin(ac, av))
		execute_buildin(&ms, av);*/
	while (true)
	{
		input = readline("\033[33mminishell\033[0m\033[35m$\033[0m ");
		if (*input)
            		add_history(input); // Add input to history
		tokens = tokenize(input); // Implement your own tokenizer
		if (!tokens)
		{
			free(input);
			continue; // In case of tokenization failure
		}
		ast = parsing_ast(tokens);
		if (!ast)
		{
			free_tokens(tokens);
			free(input);
			continue; // In case of AST parsing failure
		}
		status = execute_ast(ast);
		// Cleanup
		free_ast(ast);
		free_tokens(tokens);
		free(input);
        }
}
