/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:32:22 by emagnani          #+#    #+#             */
/*   Updated: 2024/05/24 11:04:19 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*ft_memrcpy(void *dest, const void *src, size_t n)
{
	while (n > 0)
	{
		((char *) dest)[n - 1] = ((char *) src)[n - 1];
		--n;
	}
	return (dest);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	if (dest > src)
		dest = ft_memrcpy(dest, src, n);
	else
		dest = ft_memcpy(dest, src, n);
	return (dest);
}
