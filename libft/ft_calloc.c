/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 12:17:13 by emagnani          #+#    #+#             */
/*   Updated: 2024/05/28 16:36:36 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	const size_t	size_total = nmemb * size;
	void			*ptr;

	ptr = NULL;
	if (size_total == 0 || nmemb <= SIZE_MAX / size)
		ptr = malloc(size_total);
	if (ptr != NULL)
		ft_bzero(ptr, size_total);
	return (ptr);
}
