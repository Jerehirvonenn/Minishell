#include "../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

// Function prototypes
char *build_executable(t_ast *node, t_shell *ms);
bool is_buildin(t_ast *ast);
void exec_buildin(t_shell *ms, t_ast *ast);



int	ft_exec_node(t_shell *ms, t_ast *node, bool piped);

// Function to execute a binary (external command)
static int	exec_bin(t_shell *ms, t_ast *node)
{
	int	ret;
	char	*cmd_path;

	ret = 0;
	cmd_path = build_executable(node, ms);
	if (cmd_path)
	{
		ret = execve(cmd_path, node->exp_value, ms->my_envp);
		if (ret == -1)
		{
			perror("execve");
			ms->exit_code = 1;
			exit(ms->exit_code);  // Exit the child process after failure
		}
	}
	else
	{
		fprintf(stderr, "Command not found: %s\n", node->value);
		ms->exit_code = 127;  // Set exit code to 127 (command not found)
        	exit(ms->exit_code);  // Ensure child process exits on failure
	}
	return (ret);
}

// Function to handle command execution in the child process
static void	child_process(t_shell *ms, t_ast *ast)
{
	    bool buildin = is_buildin(ast);

	    if (buildin)
		    exec_buildin(ms, ast);
	    else
		    exec_bin(ms, ast);
}


void	exec_pipe_child(t_ast *node, t_shell *ms, char direction)
{
	int	status;

	if (direction == 'l')
	{
		close(ms->pipefd[0]); // Close read end
		if (dup2(ms->pipefd[1], STDOUT_FILENO) == -1)
			perror("dup2 fails");
		close(ms->pipefd[1]); // Close write end after duplication//???

	}
	else if (direction == 'r')
	{
		close(ms->pipefd[1]);
		if (dup2(ms->pipefd[0], STDIN_FILENO) == -1)
			perror("dup2 fails");
		close(ms->pipefd[0]);
	}
	status = ft_exec_node(ms, node, true);
	exit(status);
}

void	exec_pipeline(t_ast *ast, t_shell *ms)
{
	int	pid_l;
	int	pid_r;
	int	status;


	if (pipe(ms->pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid_l = fork();
	if (!pid_l) //child
		exec_pipe_child(ast->left, ms, 'l');
	else
	{
		pid_r = fork();
		if (!pid_r) //child
			exec_pipe_child(ast->right, ms, 'r');
		else
		{
			close(ms->pipefd[0]);
			close(ms->pipefd[1]);
			waitpid(pid_l, &status, 0);
			waitpid(pid_r, &status, 0);
			return ;
		}
	}


}

int	ft_exec_node(t_shell *ms, t_ast *node, bool piped)
{
	int	cmd_pid;

	if (!node) // Base case: No more commands to execute
		return (0);
	if (node->type == T_PIPE)
	{
		fprintf(stderr, "we are here\n");
		exec_pipeline(node, ms);
	}
	else
	{
		// If it's a single command, simply execute it
		cmd_pid = fork();
		if (cmd_pid == 0)
		{
			// Child process for single command
			child_process(ms, node); // Execute command
			exit(0); // Exit child process after execution
		}
		waitpid(cmd_pid, NULL, 0); // Wait for command to finish
		return (0);
	}
	return (0);// Recursive call for the right node
}

// Main function for testing
t_ast *parsing_ast(t_token *tokens);
t_token *ft_tokenize(char *str);
void init_envp(t_shell *ms);
void print_ast_tree(t_ast *root);

int main(void) {
    t_shell ms;
    t_token *tokens;
    t_ast *ast;
    char *input;

    ms.envp_size = 0;
    ms.pwd = 0;
    init_envp(&ms);

    while (1) {
        input = readline("\033[33mminishell\033[0m\033[35m$\033[0m ");
        if (!input)
            break; // If input is NULL (like Ctrl+D), break the loop

        if (*input)
            add_history(input); // Add non-empty input to history
        tokens = ft_tokenize(input);
        ast = parsing_ast(tokens);
//	print_ast_tree(ast);

       // commands_exec(&ms, ast); // Execute the command
	ft_exec_node(&ms, ast, false);  // Execute the AST
        free(input); // Free the input after processing
        // Add any necessary cleanup for tokens and ast
    }

    return 0;
}
