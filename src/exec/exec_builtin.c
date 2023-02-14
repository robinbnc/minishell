/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:32:53 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 19:07:56 by cberganz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *exec_args)
{
	return (ft_strequ(exec_args, "exit")
		|| ft_strequ(exec_args, "echo")
		|| ft_strequ(exec_args, "cd")
		|| ft_strequ(exec_args, "pwd")
		|| ft_strequ(exec_args, "export")
		|| ft_strequ(exec_args, "unset")
		|| ft_strequ(exec_args, "env"));
}

static int8_t	set_export_var(t_list **xport, char **envp[])
{
	if (!*xport)
	{
		export_init_env(*envp, xport);
		if (*xport == NULL)
			return (-1);
	}
	return (0);
}

int	case_builtin_without_static(t_pipe_command *command,
	char **envp[], int exit, int fd)
{
	if (ft_strequ(command->exec_args[0], "exit"))
		return (close(command->save_in),
			builtin_exit(command->exec_args + 1, exit));
	else if (ft_strequ(command->exec_args[0], "echo"))
		return (builtin_echo(command->exec_args + 1, exit, fd));
	else if (ft_strequ(command->exec_args[0], "cd"))
		return (builtin_cd(command->exec_args + 1, envp, exit));
	else if (ft_strequ(command->exec_args[0], "pwd"))
		return (builtin_pwd(command->exec_args + 1, exit, fd, envp));
	else if (ft_strequ(command->exec_args[0], "env"))
		return (set_env("_", "env", envp), builtin_env(command->exec_args + 1,
				envp, exit, fd));
	return (0);
}

int	exec_builtin(t_pipe_command *command, char **envp[], int exit, int fd)
{
	static t_list	*xport;
	int				ret;

	if (set_export_var(&xport, envp))
		return (-1);
	if (command->redirection_error)
		return (command->redirection_error);
	if (!exit)
		fd = command->fd_redirection[FD_OUT];
	if (ft_strequ(command->exec_args[0], "export"))
	{
		ret = builtin_export(command->exec_args + 1, envp, fd, &xport);
		if (exit)
			free_and_exit(ret);
		return (ret);
	}
	else if (ft_strequ(command->exec_args[0], "unset"))
		return (builtin_unset(command->exec_args + 1, envp, exit, &xport));
	return (case_builtin_without_static(command, envp, exit, fd));
}
