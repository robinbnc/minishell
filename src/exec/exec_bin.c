/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 01:32:41 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/26 16:37:36 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_path_error(char *arg, int exit_status, int error)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(arg, 2);
	if (error == 1)
		ft_putendl_fd(": command not found", 2);
	if (error == 2)
		ft_putendl_fd(": permission denied", 2);
	if (error == 3)
		ft_putendl_fd(": No such file or directory", 2);
	if (error == 4)
		ft_putendl_fd(": Is a directory", 2);
	if (error == 5)
		ft_putendl_fd(DOT_ERR_MSG, 2);
	free_and_exit(exit_status);
}

static void	check_dir(t_pipe_command *command, char *path)
{
	DIR		*dir;

	dir = NULL;
	if (!ft_strequ(command->exec_args[0], ".."))
		dir = opendir(command->exec_args[0]);
	if (dir)
	{
		closedir(dir);
		print_path_error(command->exec_args[0], 126, 4);
	}
	if (!path)
		print_path_error(command->exec_args[0], 127, 1);
}

static char	*prepare_path(t_pipe_command *command, char **envp[])
{
	char	*path;

	path = NULL;
	if (command->exec_args[0] && ft_ischarset(command->exec_args[0][0],
		"./", NULL))
	{
		if (command->exec_args[0][1] == '\0'
				|| (command->exec_args[0][0] == '.'
			&& command->exec_args[0][1] == '.'
			&& command->exec_args[0][2] == '\0'))
		{
			if (ft_strequ(command->exec_args[0], ".") && !command->exec_args[1])
				print_path_error(command->exec_args[0], 2, 5);
			path = NULL;
		}
		else
			path = command->exec_args[0];
	}
	else if (command->exec_args[0] && command->exec_args[0][0] != '\0')
		path = get_path(command->exec_args, envp);
	else if (!command->exec_args[0])
		free_and_exit(0);
	check_dir(command, path);
	return (path);
}

void	exec_bin(t_pipe_command *command, char **envp[])
{
	char		*path;
	struct stat	stat;

	path = prepare_path(command, envp);
	if (lstat(path, &stat) != -1)
	{
		if ((stat.st_mode & S_IXUSR) && (stat.st_mode & S_IFREG))
		{
			execve(path, command->exec_args, *envp);
			perror("minishell: ");
			free_and_exit(1);
		}
		else
			print_path_error(command->exec_args[0], 126, 2);
	}
	else
		print_path_error(command->exec_args[0], 127, 3);
}
