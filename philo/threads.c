/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szemmour <szemmour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 09:40:59 by szemmour          #+#    #+#             */
/*   Updated: 2025/06/19 09:41:00 by szemmour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	should_stop(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->data->dead_lock);
	stop = philo->data->stop_flag;
	pthread_mutex_unlock(&philo->data->dead_lock);
	return (stop);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!should_stop(philo))
	{
		think(philo);
		eat(philo);
		sleeping(philo);
	}
	return (NULL);
}

void	philo_thread(t_philo *philos)
{
	int			i;
	pthread_t	monitor_tid;

	i = 0;
	while (i < philos->data->num_of_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_routine,
				&philos[i]) != 0)
			return (ft_clean("Thread creation error!\n", philos));
		i++;
	}
	if (pthread_create(&monitor_tid, NULL, monitor, (void *)philos) != 0)
		return (ft_clean("Monitor thread creation error!\n", philos));
	i = 0;
	while (i < philos->data->num_of_philos)
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
			return (ft_clean("Error: can't join thread!\n", philos));
		i++;
	}
	if (pthread_join(monitor_tid, NULL) != 0)
		return (ft_clean("Error: can't join monitor thread!\n", philos));
}
