#include "philo.h"

void put_message(char *message, t_philo philo)
{
	pthread_mutex_lock(&philo.data->dead_lock);
	if (philo.data->dead_flag)
	{
		pthread_mutex_unlock(&philo.data->dead_lock);
		return;
	}
	pthread_mutex_unlock(&philo.data->dead_lock);
	pthread_mutex_lock(&philo.data->write_lock);
	printf("%ld %d %s\n", get_time() - philo.data->start_time, philo.id, message);
	pthread_mutex_unlock(&philo.data->write_lock);
}

void eat(t_philo philo)
{
	
	put_message("is eating", philo);
	pthread_mutex_lock(&philo.data->meal_lock);
	philo.last_meal = get_time();
	usleep(philo.data->time_to_eat * 1000);
	pthread_mutex_unlock(&philo.data->meal_lock);
}


void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	while (1)
	{
		pthread_mutex_lock(&data->dead_lock);
		int stop = data->stop_flag;
		pthread_mutex_unlock(&data->dead_lock);

		if (stop)
			break ;
		pthread_mutex_lock(&data->eat_count_lock);
		int count = data->eat_count;
		pthread_mutex_unlock(&data->eat_count_lock);

		if (count == data->num_of_philos)
			break ;
		if (philo->id % 2 == 0)
			usleep(1000); 
		pthread_mutex_lock(philo->l_fork);
		put_message("has taken a fork", *philo);
		pthread_mutex_lock(philo->r_fork);
		put_message("has taken a fork", *philo);

		pthread_mutex_lock(&data->eat_count_lock);
		data->eat_count++;
		pthread_mutex_unlock(&data->eat_count_lock);
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		put_message("is sleeping", *philo);
		usleep(data->time_to_sleep * 1000);
		put_message("is thinking", *philo);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	t_data	*data;
	int		i;

	philos = (void *)arg;
	data = philos->data;
	i = 0;
	while (1)
	{
		while (i < data->num_of_philos)
		{
			if (get_time() - philos[i].last_meal > data->time_to_die)
			{
				put_message("died", philos[i]);
				pthread_mutex_lock(&data->dead_lock);
				data->stop_flag = 1;
				pthread_mutex_unlock(&data->dead_lock);
				return (NULL);
			}
			i++;
		}
			usleep(1000);
	}
	return (NULL);
}

void	philo_thread(t_philo *philos)
{
	int			i;
	t_data		*data;
	pthread_t	monitor;

	data = philos->data;
	i = -1;
	while (++i < data->num_of_philos)
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
	pthread_create(&monitor, NULL, monitor_routine, (void *)philos);
	i = -1;
	while (++i < data->num_of_philos)
		pthread_join(philos[i].thread, NULL);
	pthread_join(monitor, NULL);
}