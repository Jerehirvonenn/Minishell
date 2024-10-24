#include "../includes/minishell.h"

// Updated handle_cases function to manage quotes
static void	handle_cases(char **clean, char *ins, size_t *i, t_ms *ms)
{
	if (ft_isquote(ins[*i]))
	{
	//	fprintf(stderr, "quotation\n");
		handle_quoted_literal(clean, ins, i, ms);
	}
	else if (ins[*i] == '$' && ft_isdelim(ins[*i + 1]))
	{
	//	fprintf(stderr, "delimetr\n");
		handle_normal_char(clean, ins, i, ms);
	}
	else if (ins[*i] == '$' && ins[*i + 1] == '?')
	{
	//	fprintf(stderr, "exit code\n");
		handle_exit_code(clean, i, ms);
	}
	else if (ins[*i] == '$' && (ft_isalnum(ins[*i + 1]) || ins[*i + 1] == '_'))
	{
	//	fprintf(stderr, "env var\n");
		handle_envir(clean, ins, i, ms);
	}
	else
		handle_normal_char(clean, ins, i, ms);
}

// Expand argument with variable and quote handling
char	*expand_argument(char *arg, t_ms *ms)
{
	char	*expanded_arg;
	size_t	j;

	expanded_arg = ft_strdup("");
	if (!expanded_arg)
	{
		ft_free_ast(ms->ast);
		clean_ms(ms);
		printf("minishell: cannot allocate memory\n");
		exit(1);
	}
	j = 0;
	while (arg[j])
		handle_cases(&expanded_arg, arg, &j, ms);
	return (expanded_arg);
}

void	expand_ast(t_ast *node, t_ms *ms)
{
	size_t	i;
	char	*new_value;
	t_io	*temp_io;

	if (!node)
		return ;
	i = 0;
	new_value = NULL;
	while (node->exp_value && node->exp_value[i])
	{
		new_value = expand_argument(node->exp_value[i], ms);
		if (!new_value)
		{
			ft_free_ast(ms->ast);
			clean_ms(ms);
			printf("minishell: cannot allocate memory\n");
			exit(1);
		}
		free(node->exp_value[i]);
		node->exp_value[i] = new_value;
		i++;
		temp_io = node->io_list;
		while (temp_io && temp_io->value && temp_io->type != T_HEREDOC)
		{
			new_value = expand_argument(temp_io->value, ms);
			if (!new_value)
			{
				ft_free_ast(ms->ast);
				clean_ms(ms);
				printf("minishell: cannot allocate memory\n");
				exit(1);
			}
			free(temp_io->value);
			temp_io->value = new_value;
			temp_io = temp_io->next;
		}
	}
	if (node->left)
		expand_ast(node->left, ms);
	if (node->right)
		expand_ast(node->right, ms);
}
