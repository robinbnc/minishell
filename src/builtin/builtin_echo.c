/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:30:55 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/24 16:09:43 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static uint8_t	is_nflag(char *arg)
{
	if (*arg != '-' || *arg == '\0')
		return (0);
	arg++;
	if (*arg != 'n')
		return (0);
	while (*arg && *arg == 'n')
		arg++;
	if (*arg == '\0')
		return (1);
	return (0);
}

int	builtin_echo(char **exec_args, int exit, int fd)
{
	int	nflag;
	int	save_fd;

	save_fd = 0;
	nflag = 0;
	while (*exec_args && is_nflag(*exec_args))
	{
		nflag = 1;
		exec_args++;
	}
	while (*exec_args && *(exec_args + 1))
	{
		ft_putstr_fd(*exec_args, fd);
		if (*(exec_args + 1))
			ft_putstr_fd(" ", fd);
		exec_args++;
	}
	if (*exec_args)
		ft_putstr_fd(*exec_args, fd);
	if (!nflag)
		ft_putendl_fd("", fd);
	if (exit)
		free_and_exit(0);
	return (0);
}
