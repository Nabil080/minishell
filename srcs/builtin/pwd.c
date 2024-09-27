/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-tort <cle-tort@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:28:07 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/30 19:33:01 by cle-tort         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_data *data, t_cmd *cmd)
{
	char	*path;

	data->exit_code = 0;
	path = ft_getenv("PWD", data->env);
	if (path)
	{
		ft_putendl(path);
		return ;
	}
	path = getcwd(NULL, 0);
	if (!path)
		shell_error("pwd", "couldn't get current directory");
	else
		ft_putendl(path);
	free(path);
	return ;
	(void)*cmd;
}
