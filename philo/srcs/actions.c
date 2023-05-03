/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 17:31:06 by lbaumann          #+#    #+#             */
/*   Updated: 2023/05/03 16:28:56 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * not sure if > or => with death_time
 * loop that constantly supervises if the philosopher should still be alive
 * mechanism behind picking up forks
*/
void	picking_forks(t_philo *philo)
{
	while (check_change_status(philo, false))
	{
		if (get_time_elapsed(philo->data) > philo->death_time)
			time_to_die(philo);
		if (pthread_mutex_lock(&philo->data->fork_lock))
			error_philo("pf fork_lock lock failed", philo);
		if (philo->data->forks[philo->left_fork])
		{
			if (philo->data->forks[philo->right_fork])
			{
				philo->data->forks[philo->left_fork] = false;
				philo->data->forks[philo->right_fork] = false;
				if (pthread_mutex_unlock(&philo->data->fork_lock))
					error_philo("pf fork_lock unlock failed", philo);
				protected_printf("has taken a fork", YE, philo);
				protected_printf("has taken a fork", YE, philo);
				return ;
			}
		}
		if (pthread_mutex_unlock(&philo->data->fork_lock))
			error_philo("pf fork_lock unlock failed", philo);
	}
}

void	eating(t_philo *philo)
{
	if (check_change_status(philo, false))
	{
		philo->death_time = get_time_elapsed(philo->data)
			+ philo->data->time_to_die;
		protected_printf("is eating", GR, philo);
		custom_sleep(philo->data->time_to_eat, philo);
		if (pthread_mutex_lock(&philo->data->fork_lock))
			error_philo("e fork_lock lock failed", philo);
		philo->data->forks[philo->left_fork] = true;
		philo->data->forks[philo->right_fork] = true;
		if (pthread_mutex_unlock(&philo->data->fork_lock))
			error_philo("e fork_lock unlock failed", philo);
		philo->meal_count++;
	}
}

void	sleeping(t_philo *philo)
{
	if (check_change_status(philo, false))
	{
		protected_printf("is sleeping", BL, philo);
		custom_sleep(philo->data->time_to_sleep, philo);
	}
}

void	thinking(t_philo *philo)
{
	if (check_change_status(philo, false))
		protected_printf("is thinking", PU, philo);
}

/**
 * -lock death status and change still_alive to false -> so other philos
 * are stopping their respective routine
 * -print death message
*/
void	time_to_die(t_philo *philo)
{
	if (check_change_status(philo, true))
	{
		if (pthread_mutex_lock(&philo->data->printf_lock))
			error_philo("printf_lock lock failed", philo);
		printf("%s%ld %d %s%s\n", RE, get_time_elapsed(philo->data),
			philo->id, "has died", RC);
		if (pthread_mutex_unlock(&philo->data->printf_lock))
			error_philo("printf_lock lock failed", philo);
	}
}
