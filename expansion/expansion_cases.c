#include "../includes/minishell.h"

// Handle environment variable expansion
void	handle_envir(char **clean, char *ins, size_t *i, t_ms *ms)
{
	size_t	start;
	size_t	len;
	char	*envir_name;
	char	*envir_value;
	char	*temp;

	start = *i + 1;//start at next symbol after $
	while (ft_isalnum(ins[start]) || ins[start] == '_')
		start++;
	len = start - (*i + 1);//calculate the length of var
	envir_name = ft_substr(ins, *i + 1, len);//MALLOC
	if (!envir_name)
	{
		ft_free_ast(ms->ast);
		clean_ms(ms);
		free(*clean);
		printf("minishell: cannot allocate memory\n");
		exit(1);
	}
	envir_value = envp_exists(envir_name, ms);
	free(envir_name);
	if (envir_value)
	{
		temp = ft_strjoin(*clean, envir_value);
		free(*clean);
		*clean = temp;
	}
	*i = start; // Move the index after the environment variable
}

// Handle exit code expansion ($?)
void	handle_exit_code(char **clean, size_t *i, t_ms *ms)
{
	char	*substr;

	substr = ft_itoa(ms->exit_code);
	if (!substr)
	{
		ft_free_ast(ms->ast);
		clean_ms(ms);
		free(*clean);
		printf("minishell: cannot allocate memory\n");
		exit(1);
	}
	*clean = ft_strjoin(*clean, substr);
	if (!*clean)
	{
		free(substr);
		ft_free_ast(ms->ast);
		clean_ms(ms);
		free(*clean);
		printf("minishell: cannot allocate memory\n");
		exit(1);
	}
	*i += 2;
}

void	handle_normal_char(char **clean, char *ins, size_t *i, t_ms *ms)
{
	char	str[2];
	char	*temp;

	str[0] = ins[*i];
	str[1] = '\0';
	temp = ft_strjoin(*clean, str);
	if (!temp)
	{
		ft_free_ast(ms->ast);
		clean_ms(ms);
		free(*clean);
		printf("minishell: cannot allocate memory\n");
		exit(1);
	}
	free(*clean);
	*clean = temp;
	(*i)++;
}

void	handle_quoted_literal(char **clean, char *ins, size_t *i, t_ms *ms)
{
	char	quote_char;

	quote_char = ins[*i];
	(*i)++;
	while (ins[*i] && ins[*i] != quote_char)
	{
		if (quote_char == '"' && ins[*i] == '$')
		{
			// Handle variable expansion inside double quotes
			if (ft_isalnum(ins[*i + 1]) || ins[*i + 1] == '_')
				handle_envir(clean, ins, i, ms);
			else
				handle_normal_char(clean, ins, i, ms);
		}
		else
			handle_normal_char(clean, ins, i, ms);
	}
	// Move past the closing quote if found
	if (ins[*i] == quote_char)
		(*i)++;
}
