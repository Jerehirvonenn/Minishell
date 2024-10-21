#include "../includes/minishell.h"

// Updated handle_cases function to manage quotes
void	handle_cases(char **clean, char *ins, size_t *i, t_ms *ms)
{
	if (ft_isquote(ins[*i]))
	{
		//fprintf(stderr, "quotation\n");
		handle_quoted_literal(clean, ins, i, ms);
	}
	else if (ins[*i] == '$' && ft_isdelim(ins[*i + 1]))
	{
		//fprintf(stderr, "delimetr\n");
		handle_normal_char(clean, ins, i);
	}
	else if (ins[*i] == '$' && ins[*i + 1] == '?')
	{
	//	fprintf(stderr, "exit code\n");
		handle_exit_code(clean, i, ms);
	}
	else if (ins[*i] == '$' && (ft_isalnum(ins[*i + 1]) || ins[*i + 1] == '_'))
	{
		//fprintf(stderr, "env var\n");
		handle_envir(clean, ins, i, ms);
	}
	else
		handle_normal_char(clean, ins, i);
}

// Expand argument with variable and quote handling
char	*expand_argument(char *arg, t_ms *ms)
{
	char	*expanded_arg;
	size_t	j;

	expanded_arg = ft_strdup("");
	j = 0;
	if (!expanded_arg)
		return (NULL);
	while (arg[j])
	{
		//printf("Handling character: %c (at index %zu)\n", arg[j], j);
		handle_cases(&expanded_arg, arg, &j, ms);
	}
	return (expanded_arg);
}

// Expand arguments in the AST
void	expand_ast(t_ast *node, t_ms *ms)
{
	size_t	i;
	char	*expanded_arg;

	i = 0;
	if (!node || !node->exp_value)
		return ;
	while (node->exp_value[i] != NULL)
	{
		if (ft_strchr(node->exp_value[i], '$')
			|| ft_strchr(node->exp_value[i], '\"')
			|| ft_strchr(node->exp_value[i], '\''))
		{
			//printf("Expan arg: %s, from index 0\n", node->exp_value[i]);
			expanded_arg = expand_argument(node->exp_value[i], ms);
			if (expanded_arg)
			{
				free(node->exp_value[i]);
				node->exp_value[i] = expanded_arg;
			}
		}
		i++;
	}
}
