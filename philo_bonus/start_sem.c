#include "philo.h"

void	put_message(char *message, t_philo *philo)
{
	sem_wait(philo->data->write_sem);
	printf("%ld %d %s\n", (get_current_time() - philo->data->start_time), philo->id, message);
	sem_post(philo->data->write_sem);
}

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
	t_philo	*philo = (t_philo *)arg;
	
	while (1)
	{
		if (get_current_time() - philo->last_meal >= philo->data->time_to_die)
		{
			sem_wait(philo->data->write_sem);
			printf("%ld %d died\n", (get_current_time() - philo->data->start_time), philo->id);
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
		
		if (philo->data->num_times_to_eat != -1 && 
			philo->meals_eaten >= philo->data->num_times_to_eat)
			exit(0);
			
		put_message("is sleeping", philo);
		ft_usleep(philo->data->time_to_sleep);
	}
}

void	wait_children(t_philo *philos)
{
	int	i = 0;
	int	status;


	wait(&status);
	
	while (i < philos->data->num_of_philos)
	{
		if (philos[i].pid > 0)
			kill(philos[i].pid, SIGTERM);
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

void	start_sem(t_philo *philos)
{
	int i = 0;

	if (philos->data->num_of_philos == 1)
	{
		philos[0].pid = fork();
		if (philos[0].pid < 0)
		{
			ft_putstr("Error: Fork failed\n");
			return;
		}
		if (philos[0].pid == 0)
		{
			put_message("has taken a fork", &philos[0]);
			ft_usleep(philos->data->time_to_die);
			put_message("died", &philos[0]);
			exit(0);
		}
		waitpid(philos[0].pid, NULL, 0);
		return;
	}

	while (i < philos->data->num_of_philos)
	{
		philos[i].pid = fork();
		if (philos[i].pid == 0)
			philo_routine(&philos[i]);
		else if (philos[i].pid < 0)
		{
			ft_putstr("Error: Fork failed\n");
			return;
		}
		i++;
	}
	
	wait_children(philos);
}

int	init_data(t_philo *philos, t_data *data)
{
	int	i;

	sem_unlink(FORK_SEM);
	sem_unlink(WRITE_SEM);
	sem_unlink(STOP_SEM);
	sem_unlink(DEATH_SEM);
	sem_unlink(MEAL_SEM);

	data->start_time = get_current_time();
	data->forks = sem_open(FORK_SEM, O_CREAT, 0644, data->num_of_philos);
	data->write_sem = sem_open(WRITE_SEM, O_CREAT, 0644, 1);
	data->stop_sem = sem_open(STOP_SEM, O_CREAT, 0644, 0);
	data->death_sem = sem_open(DEATH_SEM, O_CREAT, 0644, 1);
	data->meal_check = sem_open(MEAL_SEM, O_CREAT, 0644, 1);
	
	if (data->forks == SEM_FAILED || data->write_sem == SEM_FAILED || 
		data->stop_sem == SEM_FAILED || data->death_sem == SEM_FAILED)
	{
		ft_putstr("Semaphore creation failed\n");
		return (0);
	}

	i = 0;
	while (i < data->num_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = data->start_time;
		philos[i].pid = 0;
		i++;
	}
	return (1);
}

void	ft_clean(t_philo *philos)
{
	if (!philos || !philos->data)
		return;
		
	sem_close(philos->data->forks);
	sem_close(philos->data->write_sem);
	sem_close(philos->data->stop_sem);
	sem_close(philos->data->death_sem);
	sem_close(philos->data->meal_check);
	
	sem_unlink(FORK_SEM);
	sem_unlink(WRITE_SEM);
	sem_unlink(STOP_SEM);
	sem_unlink(DEATH_SEM);
	sem_unlink(MEAL_SEM);
	
	free(philos->data);
	free(philos);
}