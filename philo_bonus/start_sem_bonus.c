/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_sem_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szemmour <szemmour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:38:49 by szemmour          #+#    #+#             */
/*   Updated: 2025/06/19 18:55:50 by szemmour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	eat(t_philo *philo)
{
	sem_wait(philo->data->forks);
	put_message("has taken a fork", philo);
	sem_wait(philo->data->forks);
	put_message("has taken a fork", philo);
	sem_wait(philo->data->lock_sem);
	philo->last_meal = get_current_time();
	sem_post(philo->data->lock_sem);
	put_message("is eating", philo);
	ft_usleep(philo->data->time_to_eat);
	philo->meals_eaten++;
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	*check_death(void *arg)
{
	t_philo	*philo;
	size_t	current_time;
	size_t	last_meal_time;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->data->lock_sem);
		current_time = get_current_time();
		last_meal_time = philo->last_meal;
		sem_post(philo->data->lock_sem);
		if (current_time - last_meal_time >= philo->data->time_to_die)
		{
			sem_wait(philo->data->write_sem);
			printf("%lu %d died\n", (current_time - philo->data->start_time),
				philo->id);
			exit(1);
		}
		usleep(50);
	}
	return (NULL);
}

void	philo_routine(t_philo *philo)
{
	pthread_t	death_thread;

	philo->last_meal = get_current_time();
	if (pthread_create(&death_thread, NULL, check_death, philo) != 0)
	{
		ft_putstr("Create Thread ERROR\n");
		exit(1);
	}
	if (pthread_detach(death_thread) != 0)
	{
		ft_putstr("Detach Thread ERROR\n");
		exit(1);
	}
	while (1)
	{
		put_message("is thinking", philo);
		eat(philo);
		put_message("is sleeping", philo);
		ft_usleep(philo->data->time_to_sleep);
		if (philo->data->num_times_to_eat != -1
			&& philo->meals_eaten > philo->data->num_times_to_eat)
		{
			exit(0);
		}
	}
}

void	handle_one_philo(t_philo *philos)
{
	philos[0].pid = fork();
	if (philos[0].pid < 0)
	{
		ft_putstr("Error: Fork failed\n");
		return ;
	}
	if (philos[0].pid == 0)
	{
		put_message("has taken a fork", &philos[0]);
		ft_usleep(philos->data->time_to_die);
		put_message("died", &philos[0]);
		exit(1);
	}
	waitpid(philos[0].pid, NULL, 0);
}

void	start_sem(t_philo *philos)
{
	int	i;

	if (philos->data->num_of_philos == 1)
		return (handle_one_philo(philos));
	i = 0;
	while (i < philos->data->num_of_philos)
	{
		philos[i].pid = fork();
		if (philos[i].pid == 0)
			philo_routine(&philos[i]);
		if (philos[i].pid < 0)
		{
			ft_putstr("Error: Fork failed\n");
			ft_clean(philos);
			exit(1);
		}
		i++;
	}
	wait_children(philos);
}
