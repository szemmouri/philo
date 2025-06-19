/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szemmour <szemmour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:39:17 by szemmour          #+#    #+#             */
/*   Updated: 2025/06/18 11:49:47 by szemmour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	wait_children(t_philo *philos)
{
	int	i;
	int	status;
	int	finished_proc;

	finished_proc = wait(&status);
	i = 0;
	while (i < philos->data->num_of_philos)
	{
		if (philos[i].pid > 0 && philos[i].pid != finished_proc)
			kill(philos[i].pid, SIGKILL);
		i++;
	}
	i = 0;
	while (i < philos->data->num_of_philos)
	{
		if (philos[i].pid > 0)
			waitpid(philos[i].pid, NULL, 0);
		i++;
	}
}

int	is_number(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		if (argv[i][0] == '-')
			return (0);
		if (argv[i][0] == '+')
			j++;
		while (argv[i][j])
		{
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	parce_data(char **argv, t_data *data)
{
	if (!is_number(argv))
	{
		ft_putstr("Please enter numeric arguments!\n");
		return (0);
	}
	data->num_of_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->num_times_to_eat = ft_atoi(argv[5]);
	else
		data->num_times_to_eat = -1;
	if (data->num_of_philos < 1 || data->num_of_philos > 200
		|| data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0 || (data->num_times_to_eat <= 0 && argv[5]))
	{
		ft_putstr("Enter a valid data!\n");
		return (0);
	}
	return (1);
}
