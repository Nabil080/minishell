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

void show_banner() {
    printf(COLOR_CYAN"\n"
        "  __  __ _       _     _          _ _ \n"
        " |  \\/  (_)     (_)   | |        | | |\n"
        " | \\  / |_ _ __  _ ___| |__   ___| | |\n"
        " | |\\/| | | '_ \\| / __| '_ \\ / _ \\ | |\n"
        " | |  | | | | | | \\__ \\ | | |  __/ | |\n"
        " |_|  |_|_|_| |_|_|___/_| |_|\\___|_|_|\n"
        "\n"
        "Welcome to my minimalist shell !\n"
        COLOR_RESET"Use commands, environment variables, redirections and pipes just like bash\n"
        "\n");
}

void show_prompt(int exit_code) {
    if (exit_code == 0)
        printf(COLOR_GREEN "➜ " COLOR_RESET);      // Green arrow
    else
        printf(COLOR_RED "➜ " COLOR_RESET);        // Red arrow
    
    printf(COLOR_CYAN "minishell" COLOR_RESET " > ");
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	if (argc > 2 && argv)
		exit_error("Too many arguments", NULL);
	init_data(&data, env);
    show_banner();
	while (true)
	{
		reset_data(&data);
        show_prompt(data.exit_code);
		data.line = readline("");
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
