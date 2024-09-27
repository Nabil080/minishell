/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 15:11:32 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/28 00:26:38 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_heredoc_name(size_t index)
{
	char	*name;
	char	*number;

	number = ft_itoa(index);
	if (!number)
		return (NULL);
	name = ft_strjoin_sep(".heredoc_", ".tmp", number);
	free(number);
	if (!name)
		return (NULL);
	return (name);
}

static t_cmd	*init_cmd(char *line, size_t index)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->clean_line = ft_strdup(line);
	if (!cmd->clean_line)
		return (free(cmd), NULL);
	cmd->heredoc = get_heredoc_name(index);
	if (!cmd->heredoc)
		return (free(cmd->clean_line), free(cmd), NULL);
	cmd->line = line;
	cmd->in_fd = 0;
	cmd->out_fd = 0;
	cmd->args = NULL;
	cmd->is_valid = true;
	return (cmd);
}

void	get_cmds(t_data *data)
{
	char	**cmds;
	size_t	i;

	cmds = ft_split_pipes(data->line, "|");
	if (!cmds)
		exit_error("An allocation failed", data);
	data->cmds = ft_calloc(ft_countwords_noquotes(data->line, "|") + 1,
			sizeof(t_cmd *));
	if (!data->cmds)
	{
		free(cmds);
		exit_error("An allocation failed", data);
	}
	i = 0;
	while (cmds[i])
	{
		data->cmds[i] = init_cmd(cmds[i], i);
		if (!data->cmds[i])
		{
			free_2d((void **)&cmds[i], 0);
			exit_error("Cmd allocation failed", data);
		}
		i++;
	}
	free(cmds);
}
