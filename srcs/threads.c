#include "../philo.h"

int	should_stop(t_philo *philo)
{
	int stop;
	
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
		ft_sleep(1);
	while (!should_stop(philo))
	{
		eat(philo);
		sleep(philo);
		think(philo);
	}
	return (NULL);
}

void	philo_thread(t_philo *philos)
{
	int i;
	t_data *data;
	pthread_t monitor_tid;

	data = philos->data;
	
	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]) != 0)
		{
			ft_putstr("Thread creation error!\n");
			return;
		}
		i++;
	}
	
	if (pthread_create(&monitor_tid, NULL, monitor, (void *)philos) != 0)
	{
		ft_putstr("Monitor thread creation error!\n");
		return;
	}
	
	i = 0;
	while (i < data->num_of_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_tid, NULL);
}
