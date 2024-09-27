/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:39:46 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/30 15:56:55 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	here_doc_loop(t_data *data, t_cmd *cmd, char *limiter)
{
	char	*line;
	char	*line_tmp;

	line_tmp = readline("heredoc> ");
	if (!line_tmp)
		return (false);
	line = ft_strjoin(line_tmp, "\n");
	free(line_tmp);
	if (!line)
		exit_error("Allocation failed", data);
	if (!ft_strncmp(line, limiter, ft_strlen(limiter)))
	{
		free(line);
		return (false);
	}
	ft_putstr_fd(line, cmd->in_fd);
	free(line);
	return (true);
}

static void	get_here_doc(t_data *data, t_cmd *cmd, char *limiter)
{
	int		stdin;

	stdin = dup(STDIN_FILENO);
	cmd->in_fd = open(cmd->heredoc, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	while (g_signal != 2)
	{
		if (!here_doc_loop(data, cmd, limiter))
			break ;
	}
	close(cmd->in_fd);
	cmd->in_fd = 0;
	dup2(stdin, STDIN_FILENO);
	close(stdin);
}

static void	handle_here_doc(t_data *data, t_cmd *cmd, t_parser *pars, size_t *i)
{
	char	*trimmed;
	char	*name;
	size_t	start;

	start = *i;
	while (ft_isspace(cmd->line[*i]))
		parse_str(pars, cmd->line, i);
	if (!cmd->line[*i])
		exit_error("syntax error near unexpected token '<'", data);
	while (pars->quotes || !ft_isfile_limiter(cmd->line[*i]))
		parse_str(pars, cmd->line, i);
	name = get_filename(cmd->line + start, *pars);
	if (!name)
		exit_error("malloc", data);
	trimmed = ft_strtrim(name, " ");
	free(name);
	if (!trimmed)
		exit_error("malloc", data);
	signal(SIGINT, sig_newline_heredoc);
	get_here_doc(data, cmd, trimmed);
	free(trimmed);
	signal(SIGINT, sig_newline);
}

static void	get_heredoc(t_data *data, t_cmd *cmd)
{
	t_parser	parser;
	size_t		i;

	parser = new_parser();
	i = 0;
	while (parse_str(&parser, cmd->line, &i))
	{
		if (parser.here_doc)
		{
			handle_here_doc(data, cmd, &parser, &i);
			parser.here_doc = false;
		}
	}
}

void	get_heredocs(t_data *data)
{
	size_t	i;

	i = 0;
	while (data->cmds[i])
	{
		g_signal = 0;
		get_heredoc(data, data->cmds[i]);
		i++;
	}
}
