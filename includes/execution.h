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

//exec_builtin
bool	is_builtin(t_ast *ast);
int	exec_builtin(t_ms *ms, t_ast *ast, t_exec *exec);


//execution
int	exec_bin(t_ms *ms, t_ast *node);
void	child_process(t_ms *ms, t_ast *ast, t_exec *exec);
//////
void	execute_command(t_exec *exec, t_ast *node, t_ms *ms);
void	execute_pipe(t_ast *node, t_exec *exec, t_ms *ms);
void	execute_ast(t_ast *node, t_exec *exec, t_ms *ms);

//executable
char	*build_executable(t_ast *node, t_ms *ms);
char	*find_executable(char *cmd, char *path, t_ms *ms);

//execution_utils
void	wait_for_processes(t_exec *exec);
void	wait_for_command(t_exec *exec);
int	count_commands(t_ast *node);

//envp
void	init_envp(t_ms *ms);
char	*envp_exists(char *name, t_ms *ms);

//redirections
int	ft_in(t_io *io_list);
int	ft_out(t_io *io_list);
int	ft_append(t_io *io_list);
int	redirection(t_ast *node);

//error_hanfling (for redirections)
void	error_options(int error);
void	error_handler(char *file_name, int error, int exit_status);


//builtins
int	builtin_cd(t_ms *ms, char *cmd);
void	builtin_echo(t_ms *ms, char **cmd);
void	builtin_env(t_ms *ms, int i, int j);
void	builtin_exit(t_ms *ms, char **cmd, t_exec *exec);


//EXPANSION
//utils
int	ft_isquote(char c);
int	ft_isdelim(char c);

//handle cases
void	handle_envir(char **clean, char *ins, size_t *i, t_ms *ms);
void	handle_exit_code(char **clean, size_t *i, t_ms *ms);
void	handle_normal_char(char **clean, char *ins, size_t *i);
void	handle_quoted_literal(char **clean, char *ins, size_t *i, t_ms *ms);

//expansion
void	handle_cases(char **clean, char *ins, size_t *i, t_ms *ms);
char	*expand_argument(char *arg, t_ms *ms);


#endif
