/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbicanic <rbicanic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 03:29:34 by cberganz          #+#    #+#             */
/*   Updated: 2022/03/25 15:08:36 by rbicanic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_until_equal(char *str)
{
	int		i;
	char	*export_str;

	i = 0;
	while (str[i] != '=' && str[i])
	{
		if (!ft_strncmp(&str[i], "+=", 2))
			break ;
		i++;
	}
	export_str = ft_strndup(str, MAIN, i + 1);
	if (export_str == NULL)
		return (NULL);
	return (export_str);
}

char	*str_after_equal(char *str)
{
	int		i;
	char	*after_equ_s;

	i = 0;
	while (str[i] != '=' && str[i])
		i++;
	if (str[i] == '=')
		i++;
	after_equ_s = ft_strdup(&str[i], MAIN);
	if (after_equ_s == NULL)
		return (NULL);
	return (after_equ_s);
}

t_export	*create_export_var(char *envp_el)
{
	t_export	*export_var;

	export_var = mem_alloc(sizeof(t_export) * (1), NULL, MAIN);
	export_var->var = NULL;
	export_var->value = NULL;
	export_var->equal = NULL;
	if (!export_var)
		return (NULL);
	export_var->var = str_until_equal(envp_el);
	if (export_var->var == NULL)
		return (NULL);
	if (ft_strchr(envp_el, '='))
		export_var->equal = "=";
	export_var->value = str_after_equal(envp_el);
	if (export_var->value == NULL)
		return (NULL);
	return (export_var);
}

t_list	*export_init_env(char *envp[], t_list **export_list)
{
	int			i;
	t_export	*export_var;
	t_list		*tmp;

	export_var = create_export_var(envp[0]);
	if (export_var == NULL)
		return (NULL);
	*export_list = ft_lstnew((void *)export_var, MAIN);
	i = 1;
	while (envp[i] != NULL)
	{
		export_var = create_export_var(envp[i]);
		if (export_var == NULL)
			return (NULL);
		tmp = ft_lstnew((void *)export_var, MAIN);
		if (tmp == NULL)
			return (NULL);
		ft_lstadd_back(export_list, tmp);
		i++;
	}
	return (*export_list);
}

t_list	**add_el_to_export_list(t_list **export_list, char *export_el)
{
	t_list		*tmp;
	t_export	*export_var;
	t_list		*new;

	export_var = create_export_var(export_el);
	if (export_var == NULL)
		return (NULL);
	tmp = *export_list;
	while (tmp)
	{
		if (!ft_strncmp(export_var->var, ((t_export *)tmp->content)->var,
				ft_strlen(export_var->var) + 1))
		{
			mem_remove(tmp->content, MAIN);
			tmp->content = (void *)export_var;
			return (export_list);
		}
		tmp = tmp->next;
	}
	new = ft_lstnew((void *)export_var, MAIN);
	if (new == NULL)
		return (NULL);
	ft_lstadd_back(export_list, new);
	return (export_list);
}
