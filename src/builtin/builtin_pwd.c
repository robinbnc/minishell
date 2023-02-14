/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:31:38 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 15:09:12 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(char **exec_args, int exit, int fd, char **envp[])
{
	char	*cwd;

	if (exec_args && exec_args[0] && exec_args[0][0] == '-'
		&& exec_args[0][1] != '\0')
	{
		ft_putstr_fd("minishell: pwd: -", 2);
		ft_putchar_fd(exec_args[0][1], 2);
		ft_putendl_fd(" : invalid option", 2);
		ft_putendl_fd("pwd : usage : pwd [-LP]", 2);
		if (exit)
			free_and_exit(2);
		return (2);
	}
	cwd = get_env("PWD", envp);
	if (!garbage_addptr(getcwd(NULL, 0), LOOP))
	{
		ft_putendl_fd(PWD_ERR_MSG, 2);
		return (1);
	}
	ft_putendl_fd(cwd, fd);
	if (exit)
		free_and_exit(0);
	return (0);
}
