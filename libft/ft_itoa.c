/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:00:41 by emagnani          #+#    #+#             */
/*   Updated: 2024/05/24 13:42:44 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*reverse(char *str, int len)
{
	int		start;
	int		end;
	char	tmp;

	start = 0;
	end = len - 1;
	while (start < end)
	{
		tmp = str[start];
		str[start] = str[end];
		str[end] = tmp;
		end--;
		start++;
	}
	return (str);
}

static int	nlen(long nbr)
{
	int		len;

	len = 0;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
	{
		nbr = -nbr;
		len++;
	}
	while (nbr != 0)
	{
		nbr /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	nbr;
	int		i;
	int		isneg;

	i = 0;
	nbr = n;
	isneg = (nbr < 0);
	str = malloc((nlen(nbr) + 1) * sizeof(char));
	if (!str)
		return (NULL);
	if (nbr == 0)
		str[i++] = '0';
	if (nbr < 0)
		nbr = -nbr;
	while (nbr != 0)
	{
		str[i++] = (nbr % 10) + '0';
		nbr /= 10;
	}
	if (isneg)
		str[i++] = '-';
	str[i] = '\0';
	return (reverse(str, i));
}
