/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 21:53:31 by enzo              #+#    #+#             */
/*   Updated: 2024/09/19 22:10:42 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static double	convert(const char *str, int i)
{
	double	nbr;
	double	div;

	nbr = 0.0;
	div = 1.0;
	while (str[i] && ft_isdigit(str[i]))
	{
		nbr = nbr * 10.0 + (str[i] - '0');
		i++;
	}
	if (str[i] == '.')
	{
		i++;
		while (str[i] && ft_isdigit(str[i]))
		{
			nbr = nbr * 10.0 + (str[i] - '0');
			div *= 10.0;
			i++;
		}
	}
	return (nbr / div);
}

double	ft_atof(const char *str)
{
	int		i;
	int		sign;

	sign = 1;
	i = 0;
	if (!str)
		return (0);
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	return (convert(str, i) * sign);
}
