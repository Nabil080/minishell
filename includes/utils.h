/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 00:33:17 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/30 15:56:53 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

/*init*/
void	set_signals(void);
void	init_data(t_data *data, char **env);
void	show_data(t_data data);
void	reset_data(t_data *data);
/*errors*/
void	shell_error(char *cmd, char *message);
void	exit_error(char *str, t_data *data);
void	free_data(t_data *data);
void	free_cmds(t_cmd **cmd);
void	exit_free(t_data *data);
/*env*/
char	*ft_getenv(char *name, char **env);
char	*ft_setenv(char *name, char *value, char ***env);
char	*get_filenaname(char *str);
/*checks*/
bool	is_valid_line(char *line);
bool	single_builtin(t_data *data);
size_t	count_quotes(char *str);
/*signals*/
void	sig_newline_heredoc(int sig);
void	sig_newline(int sig);
void	sig_update(int sig);

#endif