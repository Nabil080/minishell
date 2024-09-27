/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infiles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 00:26:21 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/30 18:25:18 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_outfile(t_data *data, t_cmd *cmd, t_parser *pars, char *name)
{
	char	*trimmed;

	trimmed = ft_strtrim(name, " ");
	if (!trimmed)
		exit_error("malloc", data);
	if (cmd->out_fd > 0)
		close(cmd->out_fd);
	if (pars->append)
		cmd->out_fd = open(trimmed, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else
		cmd->out_fd = open(trimmed, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (cmd->is_valid && cmd->out_fd < 0)
	{
		perror(trimmed);
		cmd->is_valid = false;
	}
	free(trimmed);
}

static void	handle_outfile(t_data *data, t_cmd *cmd, t_parser *pars, size_t *i)
{
	char	*new_line;
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
	update_outfile(data, cmd, pars, name);
	free(name);
	start--;
	if (pars->append)
		start--;
	new_line = ft_strcut(cmd->line, start, *i - 1);
	if (!new_line)
		exit_error("malloc", data);
	free(cmd->line);
	cmd->line = new_line;
}

static void	update_infile(t_data *data, t_cmd *cmd, t_parser *pars, char *name)
{
	char	*trimmed;

	trimmed = ft_strtrim(name, " ");
	if (!trimmed)
		exit_error("malloc", data);
	if (cmd->in_fd > 0)
		close(cmd->in_fd);
	if (pars->here_doc == true)
		cmd->in_fd = open(cmd->heredoc, O_RDONLY, 0777);
	else
		cmd->in_fd = open(trimmed, O_RDONLY, 0777);
	if (cmd->is_valid && cmd->in_fd < 0)
	{
		perror(trimmed);
		cmd->is_valid = false;
	}
	free(trimmed);
}

static void	handle_infile(t_data *data, t_cmd *cmd, t_parser *pars, size_t *i)
{
	char	*new_line;
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
	update_infile(data, cmd, pars, name);
	free(name);
	start--;
	if (pars->here_doc)
		start--;
	new_line = ft_strcut(cmd->line, start, *i - 1);
	if (!new_line)
		exit_error("malloc", data);
	free(cmd->line);
	cmd->line = new_line;
}

void	get_files(t_data *data, t_cmd *cmd)
{
	t_parser	parser;
	size_t		i;

	parser = new_parser();
	i = 0;
	while (cmd->is_valid && parse_str(&parser, cmd->line, &i))
	{
		if (parser.infile)
		{
			handle_infile(data, cmd, &parser, &i);
			parser = new_parser();
			i = 0;
		}
		if (parser.outfile)
		{
			handle_outfile(data, cmd, &parser, &i);
			parser = new_parser();
			i = 0;
		}
	}
}
