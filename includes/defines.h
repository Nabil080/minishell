/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tort <cle-tort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 15:51:20 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/30 19:34:25 by cle-tort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# define HERE_DOC ".here_doc.tmp"

extern int	g_signal;

typedef struct s_cmd
{
	char	*clean_line;
	char	*line;
	char	**args;
	int		in_fd;
	int		out_fd;
	char	*heredoc;
	bool	is_valid;
}	t_cmd;

typedef struct s_data
{
	t_cmd	**cmds;
	char	**env;
	char	**path;
	char	*line;
	int		pipe[2];
	int		exit_code;
	size_t	childs;
}	t_data;

typedef struct s_parser
{
	bool	quotes;
	bool	d_quotes;
	bool	s_quotes;
	bool	infile;
	bool	here_doc;
	bool	outfile;
	bool	append;
	char	*set;
}	t_parser;

#endif