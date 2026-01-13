/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_chars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 13:06:51 by emagnani          #+#    #+#             */
/*   Updated: 2024/06/23 13:06:53 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_char(int c)
{
	return (write(1, &c, 1));
}

int	print_string(char *s)
{
	int	count;

	count = 0;
	if (!s)
	{
		count = print_string("(null)");
		return (count);
	}
	while (*s)
	{
		print_char((int)*s);
		s++;
		count++;
	}
	return (count);
}
