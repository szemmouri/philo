/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szemmour <szemmour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 09:40:27 by szemmour          #+#    #+#             */
/*   Updated: 2025/06/19 09:40:28 by szemmour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	put_message("has taken a fork", philo);
	if (philo->data->num_of_philos == 1)
	{
		ft_usleep(philo->data->time_to_die);
		pthread_mutex_unlock(philo->l_fork);
		return ;
	}
	pthread_mutex_lock(philo->r_fork);
	put_message("has taken a fork", philo);
	put_message("is eating", philo);
	philo->eating = 1;
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_lock);
	ft_usleep(philo->data->time_to_eat);
	philo->eating = 0;
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

void	sleeping(t_philo *philo)
{
	put_message("is sleeping", philo);
	ft_usleep(philo->data->time_to_sleep);
}

void	think(t_philo *philo)
{
	put_message("is thinking", philo);
}
