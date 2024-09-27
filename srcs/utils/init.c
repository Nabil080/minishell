/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 14:59:26 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/30 15:50:09 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_data(t_data data)
{
	size_t	i;

	printf("\n");
	printf("Data->line : %s\n", data.line);
	i = 0;
	while (data.cmds[i])
	{
		printf("-----------Cmd[%zu]-----------\n", i);
		printf("line: %s\n", data.cmds[i]->line);
		printf("clean_line: %s\n", data.cmds[i]->clean_line);
		printf("infile : %d", data.cmds[i]->in_fd);
		printf(", outfile : %d\n", data.cmds[i]->out_fd);
		if (data.cmds[i]->args)
			ft_putarr(data.cmds[i]->args);
		printf("----------------------------\n");
		i++;
	}
	printf("\n");
}

static void	get_path(t_data *data)
{
	char	*path;

	if (data->path)
		free_2d((void **)data->path, 0);
	data->path = NULL;
	path = ft_getenv("PATH", data->env);
	if (!path)
		return ;
	data->path = ft_split(path, ":");
	if (!data->path)
		exit_error("Split allocation failed", data);
}

void	init_data(t_data *data, char **env)
{
	set_signals();
	data->env = ft_arrdup(env);
	if (!data->env)
		exit_error("An allocation failed", NULL);
	data->cmds = NULL;
	data->line = NULL;
	data->path = NULL;
	data->exit_code = 0;
	data->childs = 0;
	data->pipe[0] = 0;
	data->pipe[1] = 0;
	get_path(data);
}

void	reset_data(t_data *data)
{
	g_signal = 0;
	data->childs = 0;
	data->pipe[0] = 0;
	data->pipe[1] = 0;
	if (data->line)
		free(data->line);
	data->line = NULL;
	if (data->cmds)
		free_cmds(data->cmds);
	data->cmds = NULL;
	if (data->pipe[0])
		close(data->pipe[0]);
	if (data->pipe[1])
		close(data->pipe[1]);
	get_path(data);
}
