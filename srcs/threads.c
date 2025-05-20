#include "../philo.h"

void put_message(char *message, t_philo philo)
{

	unsigned long	timestamp;	

	timestamp = get_time() - philo.data->start_time;
	pthread_mutex_lock(&philo.data->write_lock);
	pthread_mutex_lock(&philo.data->dead_lock);
	if (philo.data->stop_flag != 1)
		printf("%ld %d %s\n", timestamp, philo.id, message);
	pthread_mutex_unlock(&philo.data->dead_lock);
	pthread_mutex_unlock(&philo.data->write_lock);
}

void eat(t_philo *philo)
{

	pthread_mutex_lock(philo->l_fork);
	put_message("has taken a fork", *philo);
	pthread_mutex_lock(philo->r_fork);
	put_message("has taken a fork", *philo);

	pthread_mutex_lock(&philo->data->meal_lock);
	philo->eating = 1;
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->meal_lock);
	
	put_message("is eating", *philo);
	ft_sleep(philo->data->time_to_eat);

	pthread_mutex_lock(&philo->data->eat_count_lock);
	philo->meals_eaten++;
	philo->eating = 0;
	pthread_mutex_unlock(&philo->data->eat_count_lock);
	
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}


void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	int     stop;
	
	philo = (t_philo *)arg;
	data = philo->data;
	
	if (philo->id % 2 == 0)
		usleep(1000);
		
	while (1)
	{
		pthread_mutex_lock(&data->dead_lock);
		stop = data->stop_flag;
		pthread_mutex_unlock(&data->dead_lock);
		
		if (stop)
			break;

		eat(philo);
		

		pthread_mutex_lock(&data->eat_count_lock);
		if (data->num_times_to_eat != -1 && philo->meals_eaten >= data->num_times_to_eat)
		{
			pthread_mutex_unlock(&data->eat_count_lock);
			break;
		}
		pthread_mutex_unlock(&data->eat_count_lock);
		
		put_message("is sleeping", *philo);
		ft_sleep(data->time_to_sleep);
		
		put_message("is thinking", *philo);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	t_data	*data;
	int		i;
	int		all_ate_enough;

	philos = (t_philo *)arg;
	data = philos->data;
	
	while (1)
	{
		i = 0;
		all_ate_enough = 1;
		
		while (i < data->num_of_philos)
		{
			pthread_mutex_lock(&data->meal_lock);
			if (get_time() - philos[i].last_meal > data->time_to_die && !philos[i].eating)
			{
				pthread_mutex_unlock(&data->meal_lock);
				put_message("died", philos[i]);
				
				pthread_mutex_lock(&data->dead_lock);
				data->stop_flag = 1;
				pthread_mutex_unlock(&data->dead_lock);
				
				return (NULL);
			}
			pthread_mutex_unlock(&data->meal_lock);
			
			pthread_mutex_lock(&data->eat_count_lock);
			if (data->num_times_to_eat != -1 && philos[i].meals_eaten < data->num_times_to_eat)
				all_ate_enough = 0;
			pthread_mutex_unlock(&data->eat_count_lock);
			
			i++;
		}
		
		if (data->num_times_to_eat != -1 && all_ate_enough)
		{
			pthread_mutex_lock(&data->dead_lock);
			data->stop_flag = 1;
			pthread_mutex_unlock(&data->dead_lock);
			return (NULL);
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