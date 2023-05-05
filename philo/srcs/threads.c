/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 17:34:43 by lbaumann          #+#    #+#             */
/*   Updated: 2023/05/05 11:19:52 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * change the valid_status to false, so other threads are exiting aswell
 * print out msg indicating what went wrong for debugging
*/
void	error_philo(char *msg, t_philo *philo)
{
	pthread_mutex_lock(&philo->data->status_lock);
	if (philo->data->valid_status)
		printf("%s\n", msg);
	philo->data->valid_status = false;
	pthread_mutex_unlock(&philo->data->status_lock);
}

/**
 * @param change if is true, the status is changed to false
 * @return the current status of valid_status
*/
bool	check_change_status(t_philo *philo, bool change)
{
	bool	status;

	if (pthread_mutex_lock(&philo->data->status_lock))
		error_philo("ttd status_lock lock failed", philo);
	status = philo->data->valid_status;
	if (change && status)
		philo->data->valid_status = false;
	if (pthread_mutex_unlock(&philo->data->status_lock))
		error_philo("ttd status_lock unlock failed", philo);
	return (status);
}

/**
 * function that every thread is calling
 * exception if there is only one philosopher -> starves to death
 * normal behaviour: infinite routine loop (as long as valid_status=true)
 * eating, sleeping, thinking
 * @param arg expects a pointer to a philo struct to identify
 * each distinct philosopher
*/
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->nphilo == 1)
	{
		protected_printf("has taken a fork", YE, philo);
		custom_sleep(philo->data->time_to_die + 100, philo);
		return (NULL);
	}
	while (check_change_status(philo, false)
		&& (philo->meal_count != philo->data->nmeals))
	{
		picking_forks(philo);
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}

/**
 * -meal_count initialized to 0
 * -left and right fork id are assigned
 * -first philosopher has right fork of last philosopher
 * -fork states are set to true -> fork available
 * -threads are created and passed the respective t_philo struct
*/
void	create_philos(t_data *data, t_philo *philos)
{
	int		i;

	i = 0;
	while (i < data->nphilo)
	{
		philos[i].id = i + 1;
		philos[i].left_fork = i;
		philos[i].data = data;
		philos[i].meal_count = 0;
		if (i == 0)
			philos[i].right_fork = data->nphilo - 1;
		else
			philos[i].right_fork = i - 1;
		philos[i].death_time = data->time_to_die;
		data->forks[i] = true;
		i++;
	}
	i = 0;
	while (i < data->nphilo)
	{
		if (pthread_create(&philos[i].tid, NULL, philo_routine, &philos[i]))
			error_fatal("pthread_create", data, philos);
		i++;
	}
}

/**
 * waits for threads to return
 * free 2 allocations in the end
*/
void	join_threads(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->nphilo)
	{
		if (pthread_join(philos[i].tid, NULL))
			error_fatal("pthread_join", data, philos);
		i++;
	}
	free (philos);
	free (data->forks);
}
