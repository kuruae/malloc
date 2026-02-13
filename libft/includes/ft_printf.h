/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 14:13:04 by emagnani          #+#    #+#             */
/*   Updated: 2024/06/23 12:50:37 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdint.h>

int	ft_printf(const char *format, ...);
int	print_char(int c);
int	print_string(char *s);
int	print_base_digit_uppercase(long nb, int base);
int	print_base_digit_lowercase(long nb, int base);
int	print_base_address(uintptr_t nb, int base);
int	print_addr(void *ptr);

#endif