/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szemmour <szemmour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:38:02 by szemmour          #+#    #+#             */
/*   Updated: 2025/06/18 11:30:14 by szemmour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
}

int	ft_atoi(const char *str)
{
	int					i;
	int					sign;
	unsigned long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign == 1 && result > 9223372036854775807)
			return (-1);
		if (sign == -1 && result > 9223372036854775807)
			return (0);
		result = (result * 10) + (str[i] - 48);
		i++;
	}
	return ((int)(result * sign));
}

void	ft_clean(t_philo *philos)
{
	if (!philos || !philos->data)
		return ;
	if (philos->data->forks != SEM_FAILED)
		sem_close(philos->data->forks);
	if (philos->data->write_sem != SEM_FAILED)
		sem_close(philos->data->write_sem);
	if (philos->data->lock_sem != SEM_FAILED)
		sem_close(philos->data->lock_sem);
	sem_unlink(FORK_SEM);
	sem_unlink(WRITE_SEM);
	sem_unlink(LOCK_SEM);
	free(philos->data);
	free(philos);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (ft_putstr("Error: can't get current time!\n"), 0);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(100);
	return (0);
}
