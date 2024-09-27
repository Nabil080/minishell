/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 00:25:51 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/30 19:42:50 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	if (argc > 2 && argv)
		exit_error("Too many arguments", NULL);
	init_data(&data, env);
	while (true)
	{
		reset_data(&data);
		data.line = readline("minishell > ");
		if (!data.line)
			exit_free(&data);
		add_history(data.line);
		if (!is_valid_line(data.line))
			continue ;
		get_vars(&data);
		get_cmds(&data);
		get_heredocs(&data);
		if (g_signal == 2)
			continue ;
		get_cleanlines(&data);
		get_args(&data);
		if (!single_builtin(&data))
			exec_cmds(&data);
	}
	return (0);
}
