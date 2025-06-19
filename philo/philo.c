/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szemmour <szemmour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 09:40:49 by szemmour          #+#    #+#             */
/*   Updated: 2025/06/19 10:25:18 by szemmour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philos(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].l_fork = &data->forks[i];
		philos[i].r_fork = &data->forks[(i + 1) % data->num_of_philos];
		philos[i].data = data;
		philos[i].eating = 0;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = data->start_time;
		i++;
	}
}

int	init_data(t_philo *philos, t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_of_philos);
	if (!data->forks)
		return (ft_putstr("Malloc error for forks!\n"), 0);
	data->stop_flag = 0;
	data->start_time = get_current_time();
	if (pthread_mutex_init(&data->dead_lock, NULL) != 0
		|| pthread_mutex_init(&data->meal_lock, NULL) != 0
		|| pthread_mutex_init(&data->write_lock, NULL) != 0)
	{
		ft_putstr("Mutex init error!\n");
		return (0);
	}
	i = 0;
	while (i < data->num_of_philos)
		if (pthread_mutex_init(&data->forks[i++], NULL) != 0)
			return (ft_putstr("Mutex init error!\n"), 0);
	init_philos(philos, data);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	*data;
	t_philo	*philos;

	if (argc < 5 || argc > 6)
		return (ft_putstr("Usage: ./philo n_p t_die t_eat t_sleep [n_meals]\n"),
			1);
	data = malloc(sizeof(t_data));
	if (!data)
		return (ft_putstr("Malloc error!\n"), 1);
	if (!parce_data(argv, data))
		return (free(data), 1);
	philos = malloc(sizeof(t_philo) * data->num_of_philos);
	if (!philos)
		return (ft_putstr("Malloc error!\n"), 1);
	if (!init_data(philos, data))
		return (ft_clean(NULL, philos), 1);
	philo_thread(philos);
	ft_clean(NULL, philos);
	return (0);
}
