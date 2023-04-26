/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:42:54 by lbaumann          #+#    #+#             */
/*   Updated: 2023/04/26 15:48:22 by lbaumann         ###   ########.fr       */
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

typedef long			t_ms;
typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				nphilo;
	int				nmeals;
	pthread_t		*tids;
	t_ms			start_time;
	t_ms			time_to_die;
	t_ms			time_to_eat;
	t_ms			time_to_sleep;
	t_philo			**philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	printf_lock;
	struct timeval	tp;
}	t_data;

typedef struct s_philo
{
	int		id;
	int		meal_count;
	t_data	*data;
}	t_philo;

//time.c
t_ms	get_time_elapsed(t_data *data);
int		ms_sleep(t_ms ms);

//utils.c
int		ft_atoi(const char *str);
size_t	ft_strlen(const char *s);
void	ft_putlong(long number);

void	error_fatal(char *e_msg, t_data *data);

#endif
