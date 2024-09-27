/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:30:21 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/26 22:21:22 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parser	new_parser(void)
{
	t_parser	parser;

	parser.quotes = false;
	parser.d_quotes = false;
	parser.s_quotes = false;
	parser.infile = false;
	parser.here_doc = false;
	parser.outfile = false;
	parser.append = false;
	parser.set = NULL;
	return (parser);
}

void	*update_parser(t_parser *parser, char c)
{
	if (c == '"' && parser->s_quotes == false)
		parser->d_quotes = !parser->d_quotes;
	else if (c == '\'' && parser->d_quotes == false)
		parser->s_quotes = !parser->s_quotes;
	parser->quotes = (parser->d_quotes || parser->s_quotes);
	if (!c)
		return (NULL);
	return (parser);
}

static void	handle_outfile(t_parser *parser, char *str, size_t *i)
{
	parser->outfile = true;
	if (str[*i + 1] == '>')
	{
		parser->append = true;
		*i += 1;
	}
	else
		parser->append = false;
}

static void	handle_infile(t_parser *parser, char *str, size_t *i)
{
	parser->infile = true;
	if (str[*i + 1] == '<')
	{
		parser->here_doc = true;
		*i += 1;
	}
	else
		parser->here_doc = false;
}

void	*parse_str(t_parser *parser, char *str, size_t	*i)
{
	update_parser(parser, str[*i]);
	if (str[*i] == '\0')
		return (NULL);
	if (str[*i] == '<' && !(parser->s_quotes || parser->d_quotes))
		handle_infile(parser, str, i);
	else if (str[*i] == '>' && !(parser->s_quotes || parser->d_quotes))
		handle_outfile(parser, str, i);
	*i += 1;
	return (str);
}
