/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 17:48:54 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/30 18:23:48 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

/*split*/
size_t		ft_countwords_noquotes(char *s, char *set);
char		**ft_split_pipes(char *s, char *set);
char		**ft_split_words(char *s, char *set);
/*cmds*/
void		get_cleanlines(t_data *data);
void		get_cmds(t_data *data);
/*vars*/
void		get_vars(t_data *data);
/*files*/
void		get_heredocs(t_data *data);
void		get_files(t_data *data, t_cmd *cmd);
/*args*/
void		get_args(t_data *data);
void		get_arg(t_data *data, t_cmd *cmd);
/*parsers*/
t_parser	new_parser(void);
void		*update_parser(t_parser *parser, char c);
void		*parse_str(t_parser *parser, char *str, size_t	*i);
/*utils*/
int			ft_istoken(int c);
int			ft_isfile_limiter(int c);
char		*get_filename(char *str, t_parser parser);

#endif