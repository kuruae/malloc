/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 14:12:51 by emagnani          #+#    #+#             */
/*   Updated: 2024/06/24 12:20:54 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	parsing_format(char format, va_list *ap)
{
	int	count;

	count = 0;
	if (format == 'c')
		count += print_char(va_arg(*ap, int));
	else if (format == 's')
		count += print_string(va_arg(*ap, char *));
	else if (format == 'p')
		count += print_addr(va_arg(*ap, void *));
	else if (format == 'd' || format == 'i' )
		count += print_base_digit_lowercase((long)va_arg(*ap, int), 10);
	else if (format == 'u')
		count += print_base_digit_lowercase(va_arg(*ap, unsigned int), 10);
	else if (format == 'x')
		count += print_base_digit_lowercase((long)va_arg(*ap, unsigned int), 16);
	else if (format == 'X')
		count += print_base_digit_uppercase((long)va_arg(*ap, unsigned int), 16);
	else if (format == '%')
		count += print_char('%');
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		count;

	va_start(ap, format);
	count = 0;
	if (!format)
		return (-1);
	while (*format)
	{
		if (*format == '%')
			count += parsing_format((*(++format)), &ap);
		else
			count += write(1, format, 1);
		format++;
	}
	va_end(ap);
	return (count);
}
