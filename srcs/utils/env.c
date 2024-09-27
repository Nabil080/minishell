/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbellila <nbellila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 18:18:19 by nbellila          #+#    #+#             */
/*   Updated: 2024/08/30 16:24:28 by nbellila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char *name, char **env)
{
	size_t	i;
	size_t	name_len;

	if (!env)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (env[i][name_len] == '=' && !ft_strncmp(name, env[i], name_len))
			return (&env[i][name_len + 1]);
		i++;
	}
	return (NULL);
}

char	*ft_setenv(char *name, char *value, char ***env)
{
	char	*str;
	size_t	i;

	str = ft_strjoin_sep(name, value, "=");
	if (!str)
		return (NULL);
	i = 0;
	while (env[0][i])
	{
		if (!ft_strncmp(name, env[0][i], ft_strlen(name)))
		{
			if (!ft_remove_index(env, i))
			{
				free(str);
				return (NULL);
			}
			break ;
		}
		i++;
	}
	if (!ft_arradd(env, str))
		return (free(str), NULL);
	return (str);
}
