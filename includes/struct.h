/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuznets <vkuznets@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:03:38 by vkuznets          #+#    #+#             */
/*   Updated: 2024/11/14 14:15:47 by vkuznets         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

//tokens
typedef enum e_token_type
{
	T_CMND,		
	T_IN_REDIR,
	T_OUT_REDIR,
	T_HERE_DOC,
	T_APEND,
	T_PIPE,
}	t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
	struct s_token		*prev;
}	t_token;

//parsing
typedef enum e_io_type
{
	T_IN,
	T_OUT,
	T_HEREDOC,
	T_APPEND,
}	t_io_type;

typedef struct s_io
{
	t_io_type	type;
	char		*value;
	int			heredoc_fd;
	int			amb_exp;
	struct s_io	*prev;
	struct s_io	*next;
}	t_io;

typedef struct s_ast
{
	t_token_type		type;
	t_io				*io_list;
	struct s_ast		*left;
	struct s_ast		*right;
	char				*value;
	char				**exp_value;
	char				*no_exp;
	int					pid;
	int					empty;
	struct s_ast		*left;
	struct s_ast		*right;
}	t_ast;

//execution
typedef struct s_exec
{
	int			pipefd[2];
	int			num_cmds;
	int			index;
	int			*pids;
}	t_exec;

//main struct
typedef struct s_ms
{
	char		**my_envp;
	int			exit_code;
	int			envp_size;
	int			stop;
	int			quit;
	int			heredoc;
	char		*pwd;
	char		*old_pwd;
	char		*tmp1;
	char		*tmp2;
	int			fds[1024];
	t_token		*tokens;
	t_ast		*ast;
}	t_ms;

typedef struct s_parsing
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe;
}	t_parsing;

#endif
