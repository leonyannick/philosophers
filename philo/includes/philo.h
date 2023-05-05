/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:42:54 by lbaumann          #+#    #+#             */
/*   Updated: 2023/05/05 11:41:07 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdio.h>
# include <stdbool.h>
# include <errno.h>
# include <sys/time.h>
# include <pthread.h>
# include <string.h>

typedef long			t_ms;
typedef struct s_philo	t_philo;

//color macro for printing status messages in color
# define GR	"\033[32;1m"
# define RE	"\033[31;1m"
# define PU	"\033[35;1m"
# define BL	"\033[34;1m"
# define YE "\033[33;1m"
# define RC "\033[0m"

/**
 * @param nphilo number of philosophers. 1st CLA
 * @param nmeals number of meals (init to 5th CLA if given, otherwise set to -1)
 * @param forks fork array with nphilo entries (fork[n]=true -> fork available,
 * fork[n] = false -> fork unavailable)
 * @param time_to_die 2nd CLA. time a philosopher survives without eating
 * @param time_to_eat 3rd CLA. time a philosopher blocks 2 forks while eating
 * @param time_to_sleep 4th CLA. time a philosopher sleeps
 * @param printf_lock mutex for printing status messages -> no mixing up
 * @param status_lock mutex for changing/reading valid_status
 * @param fork_lock mutex for changing/reading fork states
 * @param valid_status valid_state = true -> all philos alive and no error
 * has occurred
 * @param tp struct to save timestate in gettimeofday function call
*/
typedef struct s_data
{
	int				nphilo;
	int				nmeals;
	bool			*forks;
	t_ms			start_time;
	t_ms			time_to_die;
	t_ms			time_to_eat;
	t_ms			time_to_sleep;
	pthread_mutex_t	printf_lock;
	pthread_mutex_t	status_lock;
	pthread_mutex_t	fork_lock;
	bool			valid_status;
	struct timeval	tp;
}	t_data;

/**
 * @param id ranges from 1 to n philosophers. used to identify each thread
 * @param tid save thread id of every thread for thread_join function
 * @param meal_count initialized to 0 in the beginning. increased with every
 * time a philo eats
 * @param death_time time a philo should die since program start in ms.
 * updated at beginning of every meal
 * @param left_fork index in fork array of left fork (index = id - 1)
 * @param right_Fork index in fork array of right fork (index = id - 2)
 * exception: right fork of first philo: nphilo - 1
 * @param data pointer to data struct
*/
typedef struct s_philo
{
	int				id;
	pthread_t		tid;
	int				meal_count;
	t_ms			death_time;
	int				left_fork;
	int				right_fork;
	t_data			*data;
}	t_philo;

//time.c
t_ms	get_time_elapsed(t_data *data);
void	custom_sleep(t_ms sleep_time, t_philo *philo);

//utils.c
int		ft_atoi(const char *str);
size_t	ft_strlen(const char *s);
int		protected_printf(char *status, char *color, t_philo *philo);
int		ft_isdigit(int c);
int		check_valid_input(char *arg);

//actions.c
void	picking_forks(t_philo *philo);
void	eating(t_philo *philo);
void	sleeping(t_philo *philo);
void	thinking(t_philo *philo);
void	time_to_die(t_philo *philo);

//threads.c
void	*philo_routine(void *arg);
void	create_philos(t_data *data, t_philo *philos);
void	join_threads(t_data *data, t_philo *philos);
bool	check_change_status(t_philo *philo, bool change);
void	error_philo(char *msg, t_philo *philo);

//main.c
void	init_data(char **argv, int argc, t_data *data);
void	usage(void);
void	error_fatal(char *e_msg, t_data *data, t_philo *philos);

#endif
