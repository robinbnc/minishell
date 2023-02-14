/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/19 12:03:54 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 16:53:04 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_exit(int exit_code)
{
	rl_clear_history();
	del_garbage(LOOP);
	del_garbage(MAIN);
	exit(exit_code);
}

void	child_error_exit(int status_error, char *file)
{
	ft_putstr_fd("Minishell: ", 2);
	if (status_error == PATH_FILE_ERR || status_error == RIGHT_FILE_ERR
		|| status_error == EXEC_ERR || status_error == FD_ERR)
	{
		if (file)
			ft_putstr_fd(file, 2);
		perror("");
	}
	else if (status_error == PATH_CMD_ERR)
	{
		ft_putstr_fd(file, 2);
		ft_putendl_fd(": Command not found", 2);
	}
	else if (status_error == MALLOC_ERR)
		ft_putendl_fd("Allocation error", 2);
	free_and_exit(status_error);
}

void	eof_exit(void)
{
	ft_putendl_fd("exit", 2);
	free_and_exit(g_status);
}
