#include "../includes/minishell.h"

void	execute_ast(t_ast *node, t_shell *ms);
int		count_commands(t_ast *node);
void	wait_for_children(t_shell *ms, int num_children);


// Main function for testing
int	main(void)
{
	t_shell	ms;
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
		ms.num_cmds = count_commands(ast);
		ms.pids = malloc(sizeof(int) * ms.num_cmds);
		if (!ms.pids)
		{
			perror("Malloc failed for pids array");
			free(tokens); // Free tokens if malloc fails
			free(ast); // Free AST if malloc fails
			free(input);
			exit(EXIT_FAILURE);
		}
		ms.index = 0;
		print_ast_tree(ast);	
		execute_ast(ast, &ms);
		free(ms.pids);
		free(tokens); // Free tokens
		free(ast); // Free AST
		free(input); // Free input after processing
	}
	return (0);
}
