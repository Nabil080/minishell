/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 19:03:46 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/30 19:51:04 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	make_overflow(int *nb, int limit)
{
	if (*nb > limit)
		*nb = *nb % limit - 1;
	else if (*nb < 0)
		*nb = *nb % limit;
}

void	ft_exit(t_data *data, t_cmd *cmd)
{
	size_t	i;

	if (cmd->args[1])
	{
		data->exit_code = 1;
		if (cmd->args[2])
			return (shell_error("exit", "too many arguments"));
		i = 0;
		while (cmd->args[1][i])
		{
			if (!ft_isdigit(cmd->args[1][i]) && i > 0)
			{
				data->exit_code = 2;
				shell_error("exit", "numeric argument required");
				ft_putstr("exit\n");
				exit_free(data);
			}
			i++;
		}
		data->exit_code = ft_atoi(cmd->args[1]);
		make_overflow(&data->exit_code, 255);
	}
	if (data->cmds[1] == NULL)
		ft_putstr("exit\n");
	exit_free(data);
}
