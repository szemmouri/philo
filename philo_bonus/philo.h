#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <semaphore.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>

#define FORK_SEM "/forks_sem"
#define WRITE_SEM "/write_sem"
#define STOP_SEM "/stop_sem"
#define MEAL_SEM "/meal_sem"
#define DEATH_SEM "/death_sem"

typedef struct s_data
{
	size_t		time_to_die;
	size_t		time_to_eat;
	size_t		time_to_sleep;
	int			num_of_philos;
	int			num_times_to_eat;
	size_t		start_time;
	sem_t		*forks;
	sem_t		*stop_sem;
	sem_t		*write_sem;
	sem_t		*death_sem;
	sem_t		*meal_check;
}				t_data;

typedef struct s_philo
{
	pid_t		pid;
	int			id;
	int			meals_eaten;
	size_t		last_meal;
	t_data		*data;
}				t_philo;

// Functions
int		parce_data(char **argv, t_data *data);
int		ft_atoi(const char *str);
size_t	get_current_time(void);
int		ft_usleep(size_t milliseconds);
void	ft_putstr(char *str);
void	ft_clean(t_philo *philos);
void	start_sem(t_philo *philos);
void	put_message(char *message, t_philo *philo);

#endif