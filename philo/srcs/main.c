/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:52:40 by lbaumann          #+#    #+#             */
/*   Updated: 2023/04/26 16:01:06 by lbaumann         ###   ########.fr       */
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
	if (data)
	{
		//free stuff
	}
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
	if (gettimeofday(&data->tp, NULL))
		error_fatal("gettimeofday", data);
	data->start_time = data->tp.tv_sec * 1000 + data->tp.tv_usec / 1000;
	if (pthread_mutex_init(&data->printf_lock, NULL))
		error_fatal("mutex_init printf_lock", data);
}

void	thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->printf_lock);
	printf("%ld %d is thinking\n", get_time_elapsed(philo->data), philo->id);
	pthread_mutex_unlock(&philo->data->printf_lock);
}

void	sleeping(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->printf_lock);
	printf("%ld %d is sleeping\n", get_time_elapsed(philo->data), philo->id);
	pthread_mutex_unlock(&philo->data->printf_lock);
}

// void	*routine(void *ptr)
// {
// 	while (1)
// 	{
// 		eating();
// 		sleeping();
// 		thinking();
// 	}
	
// }

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	

	
	return NULL;
}

void	create_philos(t_data *data)
{
	int	i;

	i = 0;
	data->philos = malloc(sizeof(t_philo *) * data->nphilo);
	if (!data->philos)
		error_fatal("malloc philos array", data);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nphilo);
	if (!data->forks)
		error_fatal("malloc forks array", data);
	while (i < data->nphilo)
	{
		data->philos[i] = malloc(sizeof(t_philo));
		if (!data->philos[i])
			error_fatal("malloc philos array element", data);
		memset(&data->philos[i], 0, sizeof(t_philo));
		data->philos[i]->id = i + 1;
		if (pthread_mutex_init(&data->forks[i], NULL))
			error_fatal("pthread_mutex_init", data);
		i++;
	}
}

void	create_threads(t_data *data)
{
	int	i;

	i = 0;
	data->tids = malloc(sizeof(pthread_t) * data->nphilo);
	if (!data->tids)
		error_fatal("malloc tids", data);
	while (i < data->nphilo)
	{
		if (pthread_create(&data->tids[i], NULL, routine, data->philos[i]))
			error_fatal("pthread_create", data);
		i++;
	}
}

void	join_threads(t_data *data)
{
	int i = 0;
	while (i < data->nphilo)
	{
		if (pthread_join(data->tids[i], NULL))
			error_fatal("pthread_join", data);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_data data;

	// init_data(argv, argc, &data);

	data.nphilo = 3;
	create_philos(&data);
	create_threads(&data);
	join_threads(&data);

	return (EXIT_SUCCESS);
}
