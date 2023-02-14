/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 12:25:36 by rbicanic          #+#    #+#             */
/*   Updated: 2022/03/25 16:55:18 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_var_name_isalnum(char *str)
{
	if (!*str)
		return (0);
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

unsigned long	ft_strs_len(char **envp[], char *env_var)
{
	int				j;
	unsigned long	line;

	line = 0;
	j = 0;
	while ((*envp)[line])
	{
		if ((*envp)[line] == env_var)
		{
			line++;
			continue ;
		}
		j++;
		line++;
	}
	return (line);
}

char	**remove_var(char **envp[], char *env_var)
{
	int				j;
	unsigned long	line;
	char			**new_envp;

	if (mem_alloc((ft_strs_len(envp, env_var) + 1) * sizeof(char *),
			(void **)&new_envp, MAIN))
		print_message("Minishell: Allocation error.\n", RED, 1);
	line = 0;
	j = 0;
	while ((*envp)[line] != NULL)
	{
		if ((*envp)[line] == env_var)
		{
			line++;
			continue ;
		}
		new_envp[j] = (*envp)[line];
		j++;
		line++;
	}
	new_envp[j] = NULL;
	return (new_envp);
}

void	export_var_exist(t_list **export_var, char *var_name)
{
	t_list	*tmp;

	tmp = *export_var;
	if (!ft_strcmp(var_name, ((t_export *)tmp->content)->var))
	{
		*export_var = tmp->next;
		return ;
	}
	while (tmp->next)
	{
		if (!ft_strcmp(var_name, ((t_export *)tmp->next->content)->var))
		{
			tmp->next = tmp->next->next;
			return ;
		}
		tmp = tmp->next;
	}
}

int	builtin_unset(char **exec_args, char **envp[],
	int exit, t_list **export_var)
{
	char	*env_var;
	int		error;

	error = 0;
	while (*exec_args)
	{
		if ((!ft_isalpha(*exec_args[0]) && *exec_args[0] != '_')
			|| !ft_var_name_isalnum(*exec_args))
		{
			print_message("Minishell: export: «", RED, 0);
			print_message(*exec_args, RED, 0);
			print_message("» : not a valid identifier\n", RED, 0);
			error = 1;
		}
		env_var = env_variable_exist(*envp, *exec_args, ft_strlen(*exec_args));
		if (env_var)
			*envp = remove_var(envp, env_var);
		export_var_exist(export_var, *exec_args);
		exec_args++;
	}
	if (exit)
		free_and_exit(error);
	return (error);
}
