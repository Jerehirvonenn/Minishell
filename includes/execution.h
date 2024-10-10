#ifndef EXECUTION_H
# define EXECUTION_H

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <errno.h>

typedef struct s_shell
{
    char    	**my_envp;    // Environment variables
    int     	exit_code;    // Exit code for the shell
    int     	envp_size;    // Size of the environment variables
    int     	pipefd[2];    // Pipe file descriptors (read/write)
    int     	tempfd;       // Temporary file descriptor (e.g., for redirection /?
    int		    num_cmds;
    int		    index;
    char    	*pwd;         // Current working directory
    char        *old_pwd;
    int  	    *pids;        // PIDs array
    t_ast   	*ast;         // Abstract syntax tree for the command(s)
}   t_shell;


/*// Prototypes of dynamic PID array functions
void    init_pids(t_pids **pids);
void    add_pid(t_pids *pids, int pid);
void    free_pids(t_pids *pids);
void	clear_pids(t_pids *pids);*/

// Prototypes for shell execution functions
int     ft_exec_node(t_shell *ms, t_ast *node, bool piped);
void    exec_pipe_child(t_ast *node, t_shell *ms, char direction);
void    exec_pipeline(t_ast *ast, t_shell *ms);

// Parsing, tokenization, and AST-related prototypes
t_ast   *parsing_ast(t_token *tokens);
t_token *ft_tokenize(char *str);
void    print_ast_tree(t_ast *root);
void    init_envp(t_shell *ms);

// Builtin command handling and simple command
bool    is_buildin(t_ast *ast);
int	exec_buildin(t_shell *ms, t_ast *ast);
int 	exec_bin(t_shell *ms, t_ast *node);
void	child_process(t_shell *ms, t_ast *ast);

//envp
char	*envp_exists(char *name, t_shell *ms);

//Buildins
void	buildin_env(t_shell *ms, int i, int j);
void	buildin_echo(t_shell *ms, char **cmd);

// Utility function prototypes
char    *build_executable(t_ast *node, t_shell *ms);

//redirection
int	ft_in(t_io *io_list);
int	redirection(t_io *io_list);

#endif