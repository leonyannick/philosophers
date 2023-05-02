/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:42:54 by lbaumann          #+#    #+#             */
/*   Updated: 2023/05/02 18:05:15 by lbaumann         ###   ########.fr       */
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

// typedef struct s_fork
// {
// 	bool	is_taken;
// }	t_fork;

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
	pthread_mutex_t fork_lock;
	bool			valid_status;
	struct timeval	tp;
}	t_data;

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
int		ms_sleep(t_ms ms);
void	custom_sleep(t_ms sleep_time, t_philo *philo);

//utils.c
int		ft_atoi(const char *str);
size_t	ft_strlen(const char *s);
int		protected_printf(char *status, t_philo *philo);
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

void	error_fatal(char *e_msg, t_data *data);
void	join_threads(t_data *data, t_philo *philos);

#endif
