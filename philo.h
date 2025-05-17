#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_data
{
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				num_of_philos;
	int				num_times_to_eat;
	int				eat_count;
	int				stop_flag;
	int				start_time;
	int				dead_flag;
	pthread_mutex_t *forks;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	eat_count_lock;
}					t_data;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				eating;
	int				meals_eaten;
	size_t			last_meal;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	t_data			*data;
}					t_philo;

int					parce_data(char **argv, t_data *data);
void philo_thread(t_philo *philos);

// ======================= Utils =======================
int					ft_atoi(const char *str);
size_t				get_time(void);

#endif