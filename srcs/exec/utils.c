/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 19:25:33 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/30 16:07:57 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_cmd(t_cmd *cmd)
{
	ft_putstr_fd("\n---------\nExecuting : ", 2);
	ft_putendl_fd(cmd->line, 2);
	ft_putstr_fd("clean_line :", 2);
	ft_putendl_fd(cmd->clean_line, 2);
	ft_putstr_fd("in : ", 2);
	ft_putstr_fd(ft_itoa(cmd->in_fd), 2);
	ft_putstr_fd(", out : ", 2);
	ft_putendl_fd(ft_itoa(cmd->out_fd), 2);
	ft_putstr_fd("---------\n\n", 2);
}

void	wait_childs(t_data *data)
{
	signal(SIGINT, sig_update);
	while (data->childs)
	{
		waitpid(-1, &data->exit_code, 0);
		if (WIFEXITED(data->exit_code))
			data->exit_code = WEXITSTATUS(data->exit_code);
		else if (g_signal)
			data->exit_code = 128 + g_signal;
		data->childs--;
	}
	set_signals();
}

void	check_absolute_path(t_data *data, t_cmd *cmd)
{
	void	*dir;

	cmd->is_valid = false;
	dir = opendir(cmd->args[0]);
	if (dir)
	{
		shell_error(cmd->args[0], "Is a directory");
		data->exit_code = 126;
		closedir(dir);
		return ;
	}
	if (access(cmd->args[0], F_OK) == -1)
	{
		perror(cmd->args[0]);
		data->exit_code = 127;
		return ;
	}
	if (access(cmd->args[0], X_OK) == -1)
	{
		perror(cmd->args[0]);
		data->exit_code = 126;
		return ;
	}
	cmd->is_valid = true;
}
