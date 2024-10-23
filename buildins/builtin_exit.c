#include "../includes/minishell.h"

/*he exit command in a shell (like bash) is used to terminate the current shell session or a script,
and optionally return an exit status (also called a return code or exit code)*/

//!!WE DONT NEED handle ARGS but WE DO


/*
This function checks if the string is larger than LONG_MIN (-9223372036854775808) when parsed.
*/
static int	long_min_check(char *str, int i)
{
	if (str && i > 19)
		return (1); // Return 1 (indicating error) if the number is too large (more than 19 digits)
	if (i == 19 && ft_strncmp(str, "9223372036854775808", 20) > 0)
		return (1); // Check if the number exceeds the minimum possible value (LONG_MIN = -9223372036854775808)
	return (0); // Otherwise, it's valid
}

/*This function checks if the string exceeds LONG_MAX (9223372036854775807)*/
static int	long_max_check(char *str, int i)
{
	if (i > 19)
		return (1); // Return 1 (error) if the number has more than 19 digits
	if (i == 19 && ft_strncmp(str, "9223372036854775807", 20) > 0)
		return (1); // Check if the number exceeds the maximum possible value (LONG_MAX = 9223372036854775807)
	return (0); // Otherwise, it's valid
}

/*This function ensures that the input string is numeric and checks whether 
it fits within the valid range for a long integer (LONG_MIN to LONG_MAX).*/
static int	symbol_check(char *str, int i)
{
	if ((str[0] == '-' || str[0] == '+') && ft_isdigit(str[1]))
		i++; // Handle the sign: if the first character is `+` or `-` and the second character is a digit, increment `i`
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (1); // Return 1 (error) if any character is not a digit
		i++;
	}
	if (str[0] != '+' && str[0] != '-')
		return (long_max_check(str, i)); // If no sign, check against `LONG_MAX`
	else if (str[0] == '+')
		return (long_max_check(str + 1, i - 1)); // If sign is `+`, check the rest of the string against `LONG_MAX`
	else if (str[0] == '-')
		return (long_min_check(str + 1, i - 1)); // If sign is `-`, check the rest of the string against `LONG_MIN`
	return (0); // If no errors, return 0
}

void	error_exit(int errcode, t_ms *ms)
{
	(void)ms;
	//free_exit(ms);//this one suppose to exit the shell
	exit(errcode);
}

void	builtin_exit(t_ms *ms, char **cmd, t_exec *exec)
{
    int exit_code;
    int arg;

    exit_code = 0; // Default exit code is 0 (indicating success)
    if (cmd[1] && symbol_check(cmd[1], 0) == 1)
    {
        ft_putstr_fd("exit: numeric argument required\n", 2);
        error_exit(255, ms);
    }
    if (cmd[1] && cmd[2])
    {
        //not sure about this one
        ft_putstr_fd("exit: too many arguments\n", 2);
        ms->exit_code = 1;
        return ;
    }
    if (cmd[1])
    {
        arg = ft_atoi(cmd[1]);
        if (arg > -1)
            exit_code = (arg % 256);
        else
            exit_code = (256 - ((arg * -1) % 256));
    }
    if (exec->num_cmds == 1)
        ft_putstr_fd("exit\n", 1);
    error_exit(exit_code, ms);
}
