#include "../includes/minishell.h"

int	ft_isquote(char c)
{
	return (c == '\"' || c == '\'');
}

int	ft_isdelim(char c)
{
	return (c == ' ' || c == '\t');
}
