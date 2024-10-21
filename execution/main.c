#include "../includes/minishell.h"

void	print_ast_tree(t_ast *root);
t_token	*ft_tokenize(char *str);

// Main function for testing
int	main(void)
{
	t_ms	ms;
	t_exec	exec;
	t_token	*tokens;
	t_ast	*ast;
	char	*input;

	ms.envp_size = 0;
	ms.pwd = getcwd(NULL, 0);
	ms.old_pwd = 0;
	ms.exit_code = 0; //?
	init_envp(&ms);
	while (1)
	{
		input = readline("\033[33mminishell\033[0m\033[35m$\033[0m ");
		if (!input)
			// If input is NULL (like Ctrl+D), break the loop
			break ;
		if (*input)
			add_history(input); // Add non-empty input to history
		tokens = ft_tokenize(input);
		ast = parsing_ast(tokens);
		exec.num_cmds = count_commands(ast);
		exec.pids = malloc(sizeof(int) * exec.num_cmds);
		if (!exec.pids)
		{
			perror("Malloc failed for pids array");
			free(tokens); // Free tokens if malloc fails
			free(ast); // Free AST if malloc fails
			free(input);
			exit(EXIT_FAILURE);
		}
		exec.index = 0;
		//print_ast_tree(ast);
		execute_ast(ast, &exec, &ms);
		free(tokens); // Free tokens
		free(ast); // Free AST
		free(input); // Free input after processing
	}
	return (0);
}
