/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 17:46:40 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/27 20:15:11 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_error(char *cmd, char *message)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(message, 2);
}

void	exit_error(char *str, t_data *data)
{
	int	exit_code;

	if (*str)
		ft_putendl_fd(str, 2);
	if (!data)
		exit(EXIT_FAILURE);
	exit_code = data->exit_code;
	free_data(data);
	if (exit_code == 0)
		exit(EXIT_FAILURE);
	exit(exit_code);
}

void	exit_free(t_data *data)
{
	int	exit_code;

	if (!data)
		exit(EXIT_SUCCESS);
	exit_code = data->exit_code;
	free_data(data);
	exit(exit_code);
}

void	free_cmds(t_cmd **cmd)
{
	size_t	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i]->line)
			free(cmd[i]->line);
		if (cmd[i]->clean_line)
			free(cmd[i]->clean_line);
		if (cmd[i]->args)
			free_2d((void **)cmd[i]->args, 0);
		if (access(cmd[i]->heredoc, F_OK) == 0)
			unlink(cmd[i]->heredoc);
		free(cmd[i]->heredoc);
		if (cmd[i]->in_fd)
			close(cmd[i]->in_fd);
		cmd[i]->in_fd = 0;
		if (cmd[i]->out_fd)
			close(cmd[i]->out_fd);
		cmd[i]->out_fd = 0;
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

void	free_data(t_data *data)
{
	if (data->env)
		free_2d((void **)data->env, 0);
	if (data->line)
		free(data->line);
	if (data->cmds)
		free_cmds(data->cmds);
	if (data->path)
		free_2d((void **)data->path, 0);
	if (data->pipe[0])
		close(data->pipe[0]);
	if (data->pipe[1])
		close(data->pipe[1]);
}
