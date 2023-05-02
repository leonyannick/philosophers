/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 17:34:43 by lbaumann          #+#    #+#             */
/*   Updated: 2023/05/02 17:54:14 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	
	while (philo->data->still_alive)
	{
		picking_forks(philo);
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	//printf("current time: %ld id: %d left fork: %d right fork: %d death time: %ld\n", get_time_elapsed(philo->data), philo->id, philo->left_fork, philo->right_fork, philo->death_time);
	

	return NULL;
}

void	create_philos(t_data *data, t_philo *philos)
{
	int		i;

	i = 0;
	while (i < data->nphilo)
	{
		philos[i].id = i + 1;
		philos[i].left_fork = i;
		philos[i].data = data;
		if (i == 0)
			philos[i].right_fork = data->nphilo - 1;
		else
			philos[i].right_fork = i - 1;
		philos[i].death_time = data->time_to_die;
		data->forks[i] = true;
		// if (pthread_mutex_init(&philos[i].fork_lock, NULL))
		// 	error_fatal("mutex init fork lock", data);
		if (pthread_create(&philos[i].tid, NULL, philo_routine, &philos[i]))
			error_fatal("pthread_create", data);
		i++;
	}
}

void	join_threads(t_data *data, t_philo *philos)
{
	int i = 0;
	while (i < data->nphilo)
	{
		if (pthread_join(philos[i].tid, NULL))
			error_fatal("pthread_join", data);
		i++;
	}
}
