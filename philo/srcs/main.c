/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:52:40 by lbaumann          #+#    #+#             */
/*   Updated: 2023/04/30 21:37:33 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	usage(void)
{
	write(1, "Note: program needs at least 4 and maximum 5 parameters\n", 56); 
	write(1, "./philo number_of_philosophers time_to_die time_to_eat ", 55);	 
	write(1, "time_to_sleep [number_of_times_each_philosopher_must_eat]", 57);
	write(1, "\n", 1);
	exit(EXIT_SUCCESS);
}

void	error_fatal(char *e_msg, t_data *data)
{
	write(2, e_msg, ft_strlen(e_msg));
	exit(errno);
}

void	init_data(char **argv, int argc, t_data *data)
{
	// if (argc < 5 || argc > 6)
		// usage();
	data->nphilo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->nmeals = ft_atoi(argv[5]);
	else
		data->nmeals = -1;
	if (gettimeofday(&data->tp, NULL))
		error_fatal("gettimeofday", data);
	data->start_time = data->tp.tv_sec * 1000 + data->tp.tv_usec / 1000;
	if (pthread_mutex_init(&data->printf_lock, NULL))
		error_fatal("mutex_init printf_lock", data);
	if (pthread_mutex_init(&data->death_status, NULL))
		error_fatal("mutex_init death_lock", data);
	data->forks = malloc(sizeof(bool) * data->nphilo);
	if (!data->forks)
		error_fatal("malloc data->forks failed", data);
}

void	thinking(t_philo *philo)
{
	if (philo->data->still_alive)
		protected_printf("is thinking", philo);
}

void	sleeping(t_philo *philo)
{
	if (philo->data->still_alive)
	{
		protected_printf("is sleeping", philo);
		custom_sleep(philo->data->time_to_sleep, philo);	
	}
}

void	eating(t_philo *philo)
{
	t_ms	new_death_time;
	
	if (philo->data->still_alive)
	{
		new_death_time = get_time_elapsed(philo->data) + philo->data->time_to_die;
		protected_printf("is eating", philo);
		custom_sleep(philo->data->time_to_eat, philo);
		//unlock_forks
		pthread_mutex_lock(&philo->fork_lock);
		philo->data->forks[philo->left_fork] = true;
		philo->data->forks[philo->right_fork] = true;
		pthread_mutex_unlock(&philo->fork_lock);
		philo->death_time = new_death_time;
	}
}

/**
 * not sure if > or => with death_time
 * loop that constantly supervises if the philosopher should still be alive
 * mechanism behind picking up forks
*/
void	picking_forks(t_philo *philo)
{
	while (philo->data->still_alive)
	{
		if (get_time_elapsed(philo->data) > philo->death_time)
			time_to_die(philo);
		pthread_mutex_lock(&philo->fork_lock);
		if (philo->data->forks[philo->left_fork])
		{
			if (philo->data->forks[philo->right_fork])
			{
				philo->data->forks[philo->left_fork] = false;
				philo->data->forks[philo->right_fork] = false;
				pthread_mutex_unlock(&philo->fork_lock);
				protected_printf("has taken a fork", philo);
				protected_printf("has taken a fork", philo);
				return ;
			}
		}
		pthread_mutex_unlock(&philo->fork_lock);
	}
}

/**
 * -lock death status and change still_alive to false -> so other philos are stopping
 * their respective routine
 * -print death message
*/
void	time_to_die(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_status);
	philo->data->still_alive = false;
	pthread_mutex_unlock(&philo->data->death_status);
	protected_printf("has died", philo);
}




void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	
	// while (philo->data->still_alive)
	// {
	// 	picking_forks(philo);
	// 	eating(philo);
	// 	sleeping(philo);
	// 	thinking(philo);
	// }
	printf("id: %d left fork: %d right fork: %d death time: %ld\n", philo->id, philo->left_fork, philo->right_fork, philo->death_time);
	

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
		if (i == 0)
			philos[i].right_fork = data->nphilo - 1;
		else
			philos[i].right_fork = i - 1;
		philos[i].death_time = data->time_to_die;
		data->forks[i] = true;
		if (pthread_mutex_init(&philos[i].fork_lock, NULL))
			error_fatal("mutex init fork lock", data);
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

int	main(int argc, char **argv)
{
	t_data 	data;
	t_philo	*philos;

	init_data(argv, argc, &data);
	// memset(&data, 0, sizeof(t_data));
	// data.nphilo = 3;
	// philos = malloc(sizeof(t_philo) * 10);
	philos = malloc(sizeof(t_philo) * data.nphilo);
	if (!philos)
		error_fatal("malloc philos array", &data);
	
	create_philos(&data, philos);
	join_threads(&data, philos);


	return (EXIT_SUCCESS);
}
