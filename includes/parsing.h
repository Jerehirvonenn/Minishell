/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:59:04 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/14 18:37:55 by jhirvone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

/* clean_ast.c */
void		ft_free_ast(t_ast *root);
void		ft_free_ast_node(t_ast *node);
void		ft_free_io_list(t_io *io_list);

/* parsing.c */
char		**append_args(char **args, char *to_add, t_ms *ms,
				t_token **tokens);
t_ast		*parsing_ast(t_token *tokens, t_ms *ms);

/* parsing_node.c */
t_ast		*create_ast_node(t_token_type type, char *str);
t_io		*create_io_node(t_io_type type, char *value);
int			add_io(t_ast *ast_node, t_io_type io_type,
				char *io_value, t_token **tokens);

/* parsing_error_init.c */
void		parsing_malloc_failure(t_ms *ms, t_parsing *data, t_ast *node);
void		init_parsing_struct(t_parsing *data);
void		combine_pipeline(t_parsing *data);
void		hdoc_signal_failure(t_ms *ms, int fd);
t_ast		*parsing_error(t_ms *ms, t_ast *ast, t_ast *node, t_token *token);

/* heredoc.c */
int			ast_heredoc(t_ast *tree, t_ms *ms);

/* heredoc_file.c */
int			ft_createfile(int *fd_write, int *fd_read);

/* heredoc_read.c */
int			ft_heredoc_getline(char *delim, int fd_write, t_ms *ms);
void		remove_delim_quotes(char *delim);

/* parsing_utils.c */
int			heredoc_write(t_ms *ms, char *line, int fd);
int			ft_strcmp(char *s1, char *s2);
int			ft_isredirection(enum e_token_type type);
t_io_type	token_to_io_type(t_token_type type);
const char	*token_type_to_str(t_token_type type);

/* parse_cmnd.c */
t_ast		*parse_command(t_token **tokens, t_ms *ms, t_parsing *data);

#endif
