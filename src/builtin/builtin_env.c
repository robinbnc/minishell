/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:31:12 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 14:59:43 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env_error(char c)
{
	ft_putstr_fd("pwd: unrecognized option -- \'", 2);
	ft_putchar_fd(c, 2);
	ft_putendl_fd("\'", 2);
}

int	builtin_env(char **exec_args, char **envp[], int exit, int fd)
{
	int	line;

	line = 0;
	if (exec_args && exec_args[0] && exec_args[0][0] == '-' &&
		exec_args[0][1] != '\0')
	{
		print_env_error(exec_args[0][1]);
		if (exit)
			free_and_exit(125);
		return (125);
	}
	while ((*envp)[line])
	{
		if (!strcmp("_=env", (*envp)[line]))
			ft_putendl_fd("_=/usr/bin/env", fd);
		else
			ft_putendl_fd((*envp)[line], fd);
		line++;
	}
	if (exit)
		free_and_exit(0);
	return (0);
}
