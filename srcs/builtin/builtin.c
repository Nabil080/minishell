/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:23:03 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/26 22:22:08 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo"))
		return (true);
	else if (!ft_strcmp(cmd, "cd"))
		return (true);
	else if (!ft_strcmp(cmd, "pwd"))
		return (true);
	else if (!ft_strcmp(cmd, "env"))
		return (true);
	else if (!ft_strcmp(cmd, "unset"))
		return (true);
	else if (!ft_strcmp(cmd, "export"))
		return (true);
	else if (!ft_strcmp(cmd, "exit"))
		return (true);
	return (false);
}

bool	exec_builtin(t_data *data, t_cmd *cmd)
{
	if (!ft_strcmp(cmd->args[0], "echo"))
		ft_echo(data, cmd);
	else if (!ft_strcmp(cmd->args[0], "cd"))
		ft_cd(data, cmd);
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		ft_pwd(data, cmd);
	else if (!ft_strcmp(cmd->args[0], "env"))
		ft_env(data, cmd);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		ft_unset(data, cmd);
	else if (!ft_strcmp(cmd->args[0], "export"))
		ft_export(data, cmd);
	else if (!ft_strcmp(cmd->args[0], "exit"))
		ft_exit(data, cmd);
	else
		return (false);
	return (true);
}
