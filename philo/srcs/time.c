/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 11:27:02 by lbaumann          #+#    #+#             */
/*   Updated: 2023/05/03 11:26:53 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * returns the time elapsed in milliseconds since start_time
 * due to division for microseconds, time_elapsed can be off by one millisecond
*/
t_ms	get_time_elapsed(t_data *data)
{
	if (gettimeofday(&data->tp, NULL))
		error_fatal("gettimeofday", data);
	return (data->tp.tv_sec * 1000 + data->tp.tv_usec / 1000
		- data->start_time);
}

/**
 * simple function to wait milliseconds instead of microseconds
 * @param ms time in milliseconds to sleep
 * @return passes on return value of usleep function: 0=SUCCESS !0=FAILURE
*/
int	ms_sleep(t_ms ms)
{
	return (usleep(ms * 1000));
}

void	custom_sleep(t_ms sleep_time, t_philo *philo)
{
	struct timeval	tp;
	t_ms			start;
	t_ms			current;
	
	gettimeofday(&tp, NULL);
	start = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	while (return_status(philo))
	{
		gettimeofday(&tp, NULL);
		current = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		if ((current - start) >= sleep_time)
			return ;
		if (get_time_elapsed(philo->data) > philo->death_time)
			time_to_die(philo);
	}
}
