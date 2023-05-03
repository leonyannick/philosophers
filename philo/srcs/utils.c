/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 12:23:51 by lbaumann          #+#    #+#             */
/*   Updated: 2023/05/03 11:29:17 by lbaumann         ###   ########.fr       */
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

int	ft_isdigit(int c)
{
	if (c <= '9' && c >= '0')
	{
		return (1);
	}
	return (0);
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

int	check_valid_input(char *arg)
{
	int	i;

	if (!arg)
		return (-1);
	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			error_fatal("arg contains char that is no digit", NULL);
		i++;
	}
	return (ft_atoi(arg));
}

int	protected_printf(char *status, t_philo *philo)
{
	int	ret;
	if (return_status(philo))
	{
		if (pthread_mutex_lock(&philo->data->printf_lock))
			error_philo("printf_lock lock failed", philo);
		ret = printf("%ld\t%d\t%s\n", get_time_elapsed(philo->data),
			philo->id, status);
		if (pthread_mutex_unlock(&philo->data->printf_lock))
			error_philo("printf_lock unlock failed", philo);
	}
	return (ret);
}
