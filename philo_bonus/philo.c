#include "philo.h"


void	put_message(char *message, t_philo *philo)
{
	sem_wait(philo->data->write_sem);
	printf("%ld %d %s\n", (get_time() - philo->start_time), philo->id, message);
	sem_post(philo->data->write_sem);
}

void	ft_clean(t_philo *philo)
{
	sem_close(philo->data->forks);
	sem_close(philo->data->write_sem);
	sem_close(philo->data->stop_sem);
	sem_close(philo->data->meal_sem);
	sem_close(philo->data->death_sem);
	sem_unlink(FORK_SEM);
	sem_unlink(WRITE_SEM);
	sem_unlink(STOP_SEM);
	sem_unlink(MEAL_SEM);
	sem_unlink(DEATH_SEM);
	free(philo->data);
	free(philo);
}

void	init_philos(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philos)
	{
		philos[i].meal_sem = sem_open(MEAL_SEM, O_CREAT | O_EXCL, 0644, 1);
		if(philos[i].meal_sem == SEM_FAILED)
			return (ft_putstr("Semaphore feild\n"));
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = data->start_time;
		i++;
	}
}

int	init_data(t_philo *philos, t_data *data)
{
	int	i;

	data->stop_flag = 0;
	data->start_time = get_current_time();
	data->forks = sem_open(FORK_SEM, O_CREAT | O_EXCL, 0644, data->num_of_philos);
	data->write_sem = sem_open(WRITE_SEM, O_CREAT | O_EXCL, 0644, 1);
	data->stop_sem = sem_open(STOP_SEM, O_CREAT | O_EXCL, 0644, 0);
	
	if(data->forks == SEM_FAILED || data->write_sem  == SEM_FAILED || data->stop_sem == SEM_FAILED){
		ft_putstr("Semaphore feild\n");
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
		return (1);
	philos = malloc(sizeof(t_philo) * data->num_of_philos);
		if (!philos)
			return (ft_putstr("Malloc error!\n"), 1);
	if (!init_data(philos, data))
		return (1);
	start_sem(philos);
	return (0);
}
