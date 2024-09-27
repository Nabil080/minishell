/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 23:35:58 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/30 17:27:06 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_istoken(int c)
{
	return (c == '\0'
		|| c == '<' || c == '>'
		|| c == '"' || c == '\''
		|| ft_isspace(c));
}

int	ft_isfile_limiter(int c)
{
	return (c == '\0'
		|| c == '<' || c == '>'
		|| ft_isspace(c));
}

char	*get_filename(char *str, t_parser parser)
{
	char		*name;
	size_t		i;
	size_t		j;

	name = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!name)
		return (NULL);
	i = 0;
	j = 0;
	while (ft_isspace(str[i]))
		parse_str(&parser, str, &i);
	while (parser.quotes || !ft_isfile_limiter(str[i]))
	{
		if ((parser.quotes || !ft_istoken(str[i]))
			&& !((parser.d_quotes && str[i] == '"')
				|| (parser.s_quotes && str[i] == '\'')))
		{
			name[j] = str[i];
			j++;
		}
		parse_str(&parser, str, &i);
	}
	return (name);
}

static void	handle_file(t_data *data, t_cmd *cmd, t_parser *pars, size_t *i)
{
	char	*new_line;
	size_t	start;

	start = *i;
	while (ft_isspace(cmd->clean_line[*i]))
		parse_str(pars, cmd->clean_line, i);
	if (!cmd->clean_line[*i])
		exit_error("syntax error near redirection token", data);
	while (pars->quotes || !ft_isfile_limiter(cmd->clean_line[*i]))
		parse_str(pars, cmd->clean_line, i);
	start--;
	if (pars->append || pars->here_doc)
		start--;
	new_line = ft_strcut(cmd->clean_line, start, *i - 1);
	if (!new_line)
		exit_error("malloc", data);
	free(cmd->clean_line);
	cmd->clean_line = new_line;
}

void	get_cleanlines(t_data *data)
{
	t_parser	parser;
	size_t		i_cmd;
	size_t		i;

	i_cmd = 0;
	while (data->cmds[i_cmd])
	{
		parser = new_parser();
		i = 0;
		while (parse_str(&parser, data->cmds[i_cmd]->clean_line, &i))
		{
			if (parser.infile || parser.outfile)
			{
				handle_file(data, data->cmds[i_cmd], &parser, &i);
				parser = new_parser();
				i = 0;
			}
		}
		i_cmd++;
	}
}
