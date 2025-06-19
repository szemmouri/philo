/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szemmour <szemmour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:39:08 by szemmour          #+#    #+#             */
/*   Updated: 2025/06/19 10:28:23 by szemmour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	put_message(char *message, t_philo *philo)
{
	sem_wait(philo->data->write_sem);
	printf("%ld %d %s\n", (get_current_time() - philo->data->start_time),
		philo->id, message);
	sem_post(philo->data->write_sem);
}

void	init_philo(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].meals_eaten = 0;
		philos[i].pid = 0;
		i++;
	}
}

int	init_data(t_philo *philos, t_data *data)
{
	sem_unlink(FORK_SEM);
	sem_unlink(WRITE_SEM);
	sem_unlink(LOCK_SEM);
	data->start_time = get_current_time();
	data->forks = sem_open(FORK_SEM, O_CREAT | O_EXCL, 0644,
			data->num_of_philos);
	data->write_sem = sem_open(WRITE_SEM, O_CREAT | O_EXCL, 0644, 1);
	data->lock_sem = sem_open(LOCK_SEM, O_CREAT | O_EXCL, 0644, 1);
	if (data->forks == SEM_FAILED || data->write_sem == SEM_FAILED
		|| data->lock_sem == SEM_FAILED)
	{
		ft_clean(philos);
		ft_putstr("Semaphore creation failed\n");
		return (0);
	}
	init_philo(philos, data);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	*data;
	t_philo	*philos;

	if (argc < 5 || argc > 6)
	{
		ft_putstr("Usage: ./philo n_p t_die t_eat t_sleep [n_meals]\n");
		return (1);
	}
	data = malloc(sizeof(t_data));
	if (!data)
		return (ft_putstr("Malloc error!\n"), 1);
	if (!parce_data(argv, data))
		return (free(data), 1);
	philos = malloc(sizeof(t_philo) * data->num_of_philos);
	if (!philos)
		return (ft_putstr("Malloc error!\n"), free(data), 1);
	if (!init_data(philos, data))
		return (ft_clean(philos), 1);
	start_sem(philos);
	ft_clean(philos);
	return (0);
}
