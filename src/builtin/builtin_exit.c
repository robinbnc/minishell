/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:31:22 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/24 16:38:22 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	is_numeric(const char *nptr)
{
	int	len;
	int	i;

	i = 0;
	len = 0;
	while (nptr[i] == '-')
		i++;
	while ((&nptr[i])[len] && (&nptr[i])[len] >= '0' && (&nptr[i])[len] <= '9')
		len++;
	if (i > 1 || len > 19)
		return (0);
	else if (len > 17 && !i
		&& ft_strncmp("9223372036854775807", &nptr[i], len) < 0)
		return (0);
	else if (len > 17 && i
		&& ft_strncmp("9223372036854775808", &nptr[i], len) < 0)
		return (0);
	len = 0;
	while ((&nptr[i])[len])
	{
		if (!ft_isdigit((&nptr[i])[len]))
			return (0);
		len++;
	}
	return (1);
}

static uint8_t	too_many_sign(char *s)
{
	int	count;

	count = 0;
	while (*s)
	{
		if (*s == '+')
			count++;
		s++;
	}
	if (count > 1)
		return (1);
	else
		return (0);
}

int	builtin_exit(char **exec_args, int exit)
{
	int	exit_status;

	exit_status = g_status;
	if (!exit)
		ft_putendl_fd("exit", 2);
	if (exec_args[0])
	{
		if (too_many_sign(exec_args[0])
			|| !is_numeric(ft_strtrim(exec_args[0], " +", LOOP)))
		{
			ft_putstr_fd("Minishell: exit: ", 2);
			ft_putstr_fd(exec_args[0], 2);
			ft_putendl_fd(": numeric argument required", 2);
			exit_status = 2;
		}
		else if (exec_args[1])
		{
			ft_putendl_fd("Minishell: exit: too many arguments", 2);
			return (1);
		}
		else
			exit_status = ft_atoi(exec_args[0]);
	}
	free_and_exit(exit_status);
	return (exit_status);
}
