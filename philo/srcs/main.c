/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:52:40 by lbaumann          #+#    #+#             */
/*   Updated: 2023/05/03 10:38:16 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	usage(void)
{
	printf("Note: program needs at least 4 and maximum 5 parameters\n");
	printf("./philo <number_of_philosophers> <time_to_die> <time_to_eat> ");
	printf("<time_to_sleep> <[number_of_times_each_philosopher_must_eat]>\n");
	exit(EXIT_SUCCESS);
}

void	error_fatal(char *e_msg, t_data *data)
{
	if (data)
	{
		if (data->forks)
			free(data->forks);
	}
	write(2, e_msg, ft_strlen(e_msg));
	write(2, "\n", 1);
	exit(errno);
}

void	init_data(char **argv, int argc, t_data *data)
{
	if (argc < 5 || argc > 6)
		usage();
	data->nphilo = check_valid_input(argv[1]);
	data->time_to_die = check_valid_input(argv[2]);
	data->time_to_eat = check_valid_input(argv[3]);
	data->time_to_sleep = check_valid_input(argv[4]);
	if (argc == 6)
		data->nmeals = check_valid_input(argv[5]);
	else
		data->nmeals = -1;
	if (gettimeofday(&data->tp, NULL))
		error_fatal("gettimeofday", data);
	data->start_time = data->tp.tv_sec * 1000 + data->tp.tv_usec / 1000;
	if (pthread_mutex_init(&data->printf_lock, NULL))
		error_fatal("mutex_init printf_lock", data);
	if (pthread_mutex_init(&data->status_lock, NULL))
		error_fatal("mutex_init death_lock", data);
	data->forks = malloc(sizeof(bool) * data->nphilo);
	data->valid_status = true;
	if (!data->forks)
		error_fatal("malloc data->forks failed", data);
	if (pthread_mutex_init(&data->fork_lock, NULL))
		error_fatal("mutex_init fork_lock", data);
}

int	main(int argc, char **argv)
{
	t_data 	data;
	t_philo	*philos;

	init_data(argv, argc, &data);
	
	philos = malloc(sizeof(t_philo) * data.nphilo);
	if (!philos)
		error_fatal("malloc philos array", &data);
	
	create_philos(&data, philos);
	join_threads(&data, philos);
	free(data.forks);
	return (EXIT_SUCCESS);
}
