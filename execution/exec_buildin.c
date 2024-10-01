
#include "../includes/minishell.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


# include <stdbool.h>

void	buildin_echo(t_shell *ms, char **cmd);
void	buildin_pwd(t_shell *ms);
int	buildin_cd(t_shell *ms, char **args);
void	buildin_env(t_shell *ms, int i, int j);
void	exec_command(t_shell *ms, t_ast *ast);
void	commands_wait(t_shell *ms, t_ast *ast, t_ast *limit);

bool	is_buildin(t_ast *ast)
{
	char	*cmd;
	int	len;

	cmd = ast->exp_value[0]; //executable command
	len = ft_strlen(cmd);
	if (((len == 6) && !(ft_strncmp("export", cmd, len + 1))) //export with arguments only works
			|| ((len == 2) && !ft_strncmp("cd", cmd, len + 1))
			|| ((len == 5) && !ft_strncmp("unset", cmd, len + 1))
			|| ((len == 4) && !ft_strncmp("exit", cmd, len + 1))
			|| ((len == 3) && !ft_strncmp("env", cmd, len + 1)))
		return (true);
	return (false);
}

int	exec_buildin(t_shell *ms, t_ast *ast)
{
	int	ret;

	ret = 0;
	printf("Command received: %s\n", ast->exp_value[0]);
	//mb some checks before ??
	if (!ft_strncmp("echo", ast->exp_value[0], 5))
		buildin_echo(ms, ast->exp_value);
	/*else if (!ft_strncmp("cd", ast->exp_value[0], 3))
	{
		printf("Entering cd command\n");
		ret = buildin_cd(ms, ast->exp_value);
		printf("ret = %d\n", ret);
	}*/
	//i need the second one to chek if we go home or ..
	else if (!ft_strncmp("env", ast->exp_value[0], 3))
	{
		printf("Entering env command\n");
		buildin_env(ms, 0, 0);
	}
	else if (!ft_strncmp("pwd", ast->exp_value[0], 4))
	{
		printf("Entering pwd command\n");
		printf("%s\n", ms->pwd);
	//	buildin_pwd(ms);
	}
	else
		printf("Unknown command: %s\n", ast->exp_value[0]);
	ms->exit_code = ret;
	return (1);
}





// Main function for testing
t_ast	*parsing_ast(t_token *tokens);
t_token *ft_tokenize(char *str);
void	init_envp(t_shell *ms);
void	commands_exec(t_shell *ms, t_ast *ast, t_ast *prev);

int main(void)
{
    t_shell ms;
    t_token *tokens;
    t_ast *ast;
    char *input;
    int ret = 0;

    ms.envp_size = 0;
    ms.pwd = 0;
    init_envp(&ms);

    while (1)
    {
        input = readline("\033[33mminishell\033[0m\033[35m$\033[0m ");
        if (!input)
            break; // If input is NULL (like Ctrl+D), break the loop

        if (*input)
            add_history(input); // Add non-empty input to history
        tokens = ft_tokenize(input);
        ast = parsing_ast(tokens);
        // Execute the command (actual execution)
        //exec_bin(&ms, ast);
	//exec_buildin(&ms, ast);
	//exec_fork(&ms, ast);
	//exec_piped(&ms, ast);
	commands_exec(&ms, ast, ast);
    }
}
