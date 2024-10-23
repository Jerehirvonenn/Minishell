#include "../includes/minishell.h"
#include <stdbool.h>

static int	check_flags(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-')
		return (1);
	else
		i++;
	while (str[i])
	{
		if (str[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

void	builtin_echo(t_ms *ms, char **cmd)
{
	(void)ms;
	int	i;
	bool	flag;

	i = 1;
	flag = false;
	//if check_flags returns 0 (indicating a valid -n flag), it sets n_flag to true and moves to the next argument.
	while (cmd[i] && check_flags(cmd[i]) == 0)
	{
		flag = true;
		i++;
	}
	//If no arguments are left (cmd[i] == NULL), and the -n flag was not set, it prints a newline (\n) using ft_putchar_fd, ensuring that no extra text is output
	if (!cmd[i] && flag == false)
		ft_putchar_fd('\n', 1);
	while (cmd[i])
	{
		ft_putstr_fd(cmd[i], 1);
		if (cmd[i + 1] == NULL && flag == false)
			ft_putchar_fd('\n', 1);
		else if (cmd[i + 1] != NULL)
			ft_putchar_fd(' ', 1);
		i++;
	}
}

/*int main(int argc, char **argv) {
    t_shell ms;

    if (argc > 1) 
        mini_echo(&ms, argv);
    else
        printf("Usage: ./echo [text] [-n]\n");

    return 0;
}*/
