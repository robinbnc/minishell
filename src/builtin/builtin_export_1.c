/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 03:29:34 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 15:01:02 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_var_name_isalnum(char *str)
{
	if (!*str)
		return (0);
	while (*str && *str != '=' && *str != '_')
	{
		if (!ft_strncmp(str, "+=", 2))
			return (2);
		if (!ft_isalnum(*str))
			return (0);
		str++;
	}
	return (1);
}

char	*env_variable_exist(char *envp[], char *param, size_t var_name_len)
{
	while (*envp)
	{
		if (!ft_strncmp(param, *envp, var_name_len))
			return (*envp);
		envp++;
	}
	return (*envp);
}

uint8_t	add_var_to_env(char *exec_args, char **envp[],
	char *equal_ptr, int ret_alnum)
{
	char	*var_name;
	char	*var_value;
	char	*var_ptr_env;

	var_ptr_env = NULL;
	if (ret_alnum == 2)
	{
		var_name = ft_strndup(exec_args, MAIN, (equal_ptr - exec_args));
		var_ptr_env = env_variable_exist(*envp, var_name, ft_strlen(var_name));
	}	
	else
		var_name = ft_strndup(exec_args, MAIN, (equal_ptr - exec_args) + 1);
	if (var_name == NULL)
		return (1);
	if (ret_alnum == 2 && var_ptr_env)
		var_value = ft_strjoin(&var_ptr_env[ft_strlen(var_name) + 1],
				equal_ptr + 1, MAIN);
	else
		var_value = ft_strndup(equal_ptr + 1,
				MAIN, ft_strlen(equal_ptr + 1) + 1);
	if (var_value == NULL)
		return (1);
	set_env(var_name, var_value, envp);
	return (0);
}

int	export_exec(int *error, t_list **export_var, char **envp[], char *exec_args)
{
	int					ret_name_alnum;
	char				*equal_ptr;

	ret_name_alnum = ft_var_name_isalnum(exec_args);
	if ((!ft_isalpha(exec_args[0]) && exec_args[0] != '_') || !ret_name_alnum)
	{
		print_message("Minishell: export: «", RED, 0);
		print_message(exec_args, RED, 0);
		print_message("» : not a valid identifier\n", RED, 0);
		*error = 1;
		return (0);
	}
	if (!add_el_to_export_list(export_var, exec_args))
		return (-1);
	equal_ptr = ft_strchr(exec_args, '=');
	if (equal_ptr && add_var_to_env(exec_args, envp, equal_ptr, ret_name_alnum))
		return (1);
	return (0);
}

int	builtin_export(char **exec_args, char **envp[],
	int fd, t_list **export_var)
{
	int	error;
	int	ret;

	error = 0;
	if (!(*exec_args))
	{
		print_strs_fd(export_var, fd);
		return (0);
	}
	while (*exec_args)
	{
		ret = export_exec(&error, export_var, envp, *exec_args);
		if (ret == 1 || ret == -1)
			return (ret);
		exec_args++;
	}
	return (error);
}
