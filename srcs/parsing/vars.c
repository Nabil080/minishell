/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tort <cle-tort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:34:17 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/30 19:33:09 by cle-tort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_varindex(char *line)
{
	t_parser	parser;
	size_t		i;

	parser = new_parser();
	i = 0;
	while (line[i])
	{
		update_parser(&parser, line[i]);
		if (!parser.s_quotes && line[i] == '$' && (ft_isalnum(line[i + 1])
				|| line[i + 1] == '?'))
			return (i);
		i++;
	}
	return (-42);
}

static char	*get_varname(t_data *data, char *line)
{
	char	*name;
	size_t	len;

	if (line[0] == '?')
		len = 1;
	else
	{
		len = 0;
		while (ft_isalnum(line[len]))
			len++;
	}
	name = ft_substr(line, 0, len);
	if (!name)
		exit_error("Name alloc failed", data);
	return (name);
}

static void	*get_newline(t_data *data, size_t index, char *value, char *name)
{
	char	*new;
	size_t	len;

	len = 0;
	while (ft_isalnum(name[len]))
		len++;
	if (len == 0 && name[len] == '?')
		len = 1;
	new = ft_calloc(ft_strlen(data->line) - len
			+ ft_strlen(value), sizeof(char));
	if (!new)
		return (NULL);
	ft_strlcat(new, data->line, index + 1);
	if (len)
		ft_strcat(new, value);
	else
		ft_strcat(new, "$");
	ft_strcat(new, &data->line[index + 1 + len]);
	free(data->line);
	data->line = new;
	return (new);
}

void	get_vars(t_data *data)
{
	char	*name;
	char	*value;
	int		index;

	index = get_varindex(data->line);
	while (index >= 0)
	{
		name = get_varname(data, &data->line[index + 1]);
		value = ft_getenv(name, data->env);
		if (!ft_strcmp(name, "?"))
			value = ft_itoa(data->exit_code);
		if ((!ft_strcmp(name, "?") && !value)
			|| !get_newline(data, index, value, name))
		{
			if (!ft_strcmp(name, "?") && value)
				free(value);
			free(name);
			exit_error("An allocation failed", data);
		}
		if (!ft_strcmp(name, "?"))
			free(value);
		free(name);
		index = get_varindex(data->line);
	}
}
