/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_str_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:37:12 by emagnani          #+#    #+#             */
/*   Updated: 2024/08/22 11:48:57 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Frees memory of a dynamically allocated array of strings
* and sets the original pointer to NULL (avoids dangling pointers)
*
* To use this function you must pass the address of the array
* and not pass it by value, so use ft_free_str_array(&array);
* The point of that is to make the pointer NULL outside of
* the function's scope. 
*
* The function takes a triple pointer so it can take a pointer
* to the address of a double pointer variable after dereferencing it,
* so a char **array and not a char ***array. */

void	ft_free_str_array(char ***array)
{
	size_t	i;

	i = 0;
	if (!array)
		return ;
	while ((*array)[i])
	{
		free((*array)[i]);
		((*array)[i]) = NULL;
		i++;
	}
	free(*array);
	*array = NULL;
}
