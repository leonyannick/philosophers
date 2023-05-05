/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 11:27:02 by lbaumann          #+#    #+#             */
/*   Updated: 2023/05/05 11:28:35 by lbaumann         ###   ########.fr       */
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
		error_fatal("gettimeofday", data, NULL);
	return (data->tp.tv_sec * 1000 + data->tp.tv_usec / 1000
		- data->start_time);
}

/**
 * the function stays in the loop for sleep_time seconds
 * and constantly checks if the valid_status is still set to true
 * valid_status=false would mean that either a philo died or an error
 * occurred
*/
void	custom_sleep(t_ms sleep_time, t_philo *philo)
{
	struct timeval	tp;
	t_ms			start;
	t_ms			current;

	if (gettimeofday(&tp, NULL))
		error_philo("cs gettimeofday", philo);
	start = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	while (check_change_status(philo, false))
	{
		if (gettimeofday(&tp, NULL))
			error_philo("cs gettimeofday", philo);
		current = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		if ((current - start) >= sleep_time)
			return ;
		if (get_time_elapsed(philo->data) >= philo->death_time)
			time_to_die(philo);
	}
}
