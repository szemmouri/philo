#include "philo.h"

void	put_message(char *message, t_philo *philo)
{
	if (!should_stop(philo))
	{
		pthread_mutex_lock(&philo->data->write_lock);
		printf("%ld %d %s\n", get_current_time() - philo->data->start_time,
			philo->id, message);
		pthread_mutex_unlock(&philo->data->write_lock);
	}
}

int	is_philo_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->meal_lock);
	if (get_current_time() - philo->last_meal >= philo->data->time_to_die
		&& philo->eating == 0)
		return (pthread_mutex_unlock(&philo->data->meal_lock), 1);
	pthread_mutex_unlock(&philo->data->meal_lock);
	return (0);
}

int	is_any_philo_deid(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->data->num_of_philos)
	{
		if (is_philo_dead(&philos[i]))
		{
			put_message("died", &philos[i]);
			pthread_mutex_lock(&philos->data->dead_lock);
			philos->data->stop_flag = 1;
			pthread_mutex_unlock(&philos->data->dead_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

int	is_all_philos_ete(t_philo *philos)
{
	int	i;
	int	eat_count;

	i = 0;
	eat_count = 0;
	if (philos->data->num_times_to_eat == -1)
		return (0);
	while (i < philos->data->num_of_philos)
	{
		pthread_mutex_lock(&philos->data->meal_lock);
		if (philos[i].meals_eaten >= philos->data->num_times_to_eat)
			eat_count = 1;
		else 
			eat_count = 0;
		pthread_mutex_unlock(&philos->data->meal_lock);
		i++;
	}
	if (eat_count)
	{
		pthread_mutex_lock(&philos->data->dead_lock);
		philos->data->stop_flag = 1;
		pthread_mutex_unlock(&philos->data->dead_lock);
		return (1);
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_philo *philos = (t_philo *)arg;

	while (1)
	{
		if (is_any_philo_deid(philos) || is_all_philos_ete(philos))
			break ;
	}
	return (NULL);
}