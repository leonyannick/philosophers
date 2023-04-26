/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 12:23:51 by lbaumann          #+#    #+#             */
/*   Updated: 2023/04/25 17:24:17 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_atoi(const char *str)
{
	int	num;
	int	sign;

	num = 0;
	sign = 1;
	while ((*str >= '\t' && *str <= '\r') || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str <= '9' && *str >= '0')
	{
		num = 10 * num + *str - '0';
		str++;
	}
	num *= sign;
	return (num);
}

size_t	ft_strlen(const char *s)
{
	size_t	size;

	size = 0;
	if (!s)
		return (size);
	while (*s != 0)
	{
		size++;
		s++;
	}
	return (size);
}

void	ft_putlong(long number)
{
	char	base10[] = "0123456789";

	if (number == -9223372036854775808ULL)
	{
		write(1, "-9223372036854775808", 20);
		return ;
	}
	if (number < 0)
	{
		write(1, "-", 1);
		number = -number;
	}
	if (number > 9)
		ft_putlong(number / 10);
	write(1, &base10[number % 10], 1);
}
