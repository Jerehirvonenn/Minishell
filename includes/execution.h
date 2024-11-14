/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 12:07:45 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/14 13:13:18 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

// Libraries
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>

//EXPANSION
char	*expand_argument(char *arg, t_ms *ms);
void	mini_exp(t_ms *ms, t_ast *node);
void	expand_ast(t_ast *node, t_ms *ms);
void	handle_envir(char **clean, char *ins, size_t *i, t_ms *ms);
void	handle_exit_code(char **clean, size_t *i, t_ms *ms);
void	handle_normal_char(char **clean, char *ins, size_t *i, t_ms *ms);
void	handle_quoted_literal(char **clean, char *ins, size_t *i, t_ms *ms);
void	remove_string_i(char **array, int i);
int		ft_isquote(char c);
int		ft_isdelim(char c);

//BUILTINS
int		builtin_cd(t_ms *ms, t_ast *ast, char *cmd);
//cd_utils.c
int		check_cd_args(t_ast *ast, t_ms *ms);
int		handle_cd_error(char *cmd, t_ms *ms);
int		check_filename_length(char *cmd, t_ms *ms);
void	handle_pwd_errors(t_ms *ms, char *cmd);

void	builtin_echo(char **cmd);
void	builtin_env(t_ms *ms);
void	builtin_exit(t_ms *ms, char **cmd);
int		long_min_check(char *str, int i);
int		long_max_check(char *str, int i);
void	builtin_export(t_ms *ms, char **cmd, int i);
void	builtin_unset(t_ms *ms, char **cmd);

//ENVP
void	envp_update(t_ms *ms, char *content);
int		envp_remove(t_ms *ms, char *content);
void	init_envp(t_ms *ms, char **envp);
char	*envp_exists(char *name, t_ms *ms);
char	*name_exists(const char *arg, t_ms *ms);
void	update_pwds_envp(t_ms *ms);
char	*find_the_path(char *str, t_ms *ms);

//EXECUTION
void	child_process(t_ms *ms, t_ast *ast);
void	execute_ast(t_ast *node, t_ms *ms);
bool	is_builtin(t_ast *ast);
int		exec_builtin(t_ms *ms, t_ast *ast);
//execute_pipe.c
void	execute_first_pipe(t_ast *node, t_ms *ms);
void	execute_pipe(t_ast *node, t_ms *ms, int *wr_p);
void	execute_first_command(t_ast *node, t_ms *ms, int *pipefd);
void	execute_last_command(t_ast *node, t_ms *ms, int *pipefd);
void	execute_middle_command(t_ast *node, t_ms *ms, int *pipefd, int *wr_p);
//exec_bin.c
int		exec_bin(t_ms *ms, t_ast *node);
//utils
void	close_fd(int *close_fd);
void	close_and_change_array(t_ms *ms, int *fd);
void	close_multiple_fds(int *pipefd);
void	close_array_fds(t_ms *ms);
void	add_to_array(t_ms *ms, int fd);
//utils2
void	handle_fork_failure(t_ms *ms);
void	handle_dup2_failure(t_ms *ms);
void	clean_and_exit(t_ms *ms, int exit_code);

char	*build_executable(t_ast *node, t_ms *ms);

//REDIRECTION
void	error_options(int error);
void	error_handler(char *file_name, int error);
int		check_file_access(char *filename, int access_mode, int error_code);
int		redirection(t_ast *node);
int		redirection_parent(t_ast *node);

//CLEAN_UTILS
void	malloc_parent_failure(t_ms *ms);
void	handle_allocation_failure(char **new_envp, int count, t_ms *ms);

#endif
