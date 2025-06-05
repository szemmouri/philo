#include "philo_bonus.h"

void	eat(t_philo *philo)
{
	sem_wait(philo->data->forks);
	put_message("has taken a fork", philo);
	sem_wait(philo->data->forks);
	put_message("has taken a fork", philo);
	philo->last_meal = get_current_time();
	put_message("is eating", philo);
	ft_usleep(philo->data->time_to_eat);
	philo->meals_eaten++;
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	*check_death(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (get_current_time() - philo->last_meal >= philo->data->time_to_die)
		{
			sem_wait(philo->data->write_sem);
			printf("%ld %d died\n", (get_current_time()
					- philo->data->start_time), philo->id);
			exit(1);
		}
		usleep(1000);
	}
	return (NULL);
}

void	philo_routine(t_philo *philo)
{
	pthread_t	death_thread;

	pthread_create(&death_thread, NULL, &check_death, philo);
	pthread_detach(death_thread);
	while (1)
	{
		put_message("is thinking", philo);
		eat(philo);
		if (philo->data->num_times_to_eat != -1
			&& philo->meals_eaten >= philo->data->num_times_to_eat)
			exit(0);
		put_message("is sleeping", philo);
		ft_usleep(philo->data->time_to_sleep);
	}
}

void	handel_one_philo(t_philo *philos)
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
		exit(0);
	}
	waitpid(philos[0].pid, NULL, 0);
}

void	start_sem(t_philo *philos)
{
	int	i;

	i = 0;
	if (philos->data->num_of_philos == 1)
		return (handel_one_philo(philos));
	while (i < philos->data->num_of_philos)
	{
		philos[i].pid = fork();
		if (philos[i].pid == 0)
			philo_routine(&philos[i]);
		else if (philos[i].pid < 0)
		{
			ft_putstr("Error: Fork failed\n");
			return ;
		}
		i++;
	}
	wait_children(philos);
}
