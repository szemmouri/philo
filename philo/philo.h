/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szemmour <szemmour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 09:42:13 by szemmour          #+#    #+#             */
/*   Updated: 2025/06/19 09:42:14 by szemmour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				num_of_philos;
	int				num_times_to_eat;
	int				stop_flag;
	size_t			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	write_lock;
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

// ======================= Parcing =======================
int					parce_data(char **argv, t_data *data);
void				philo_thread(t_philo *philos);

// ======================= Utils =======================
int					ft_atoi(const char *str);
size_t				get_current_time(void);
int					ft_usleep(size_t milliseconds);
void				ft_clean(char *str, t_philo *philos);
void				ft_putstr(char *str);

// ======================= Actions =======================
void				eat(t_philo *philo);
void				sleeping(t_philo *philo);
void				think(t_philo *philo);

// ======================= Monitor =======================
void				*monitor(void *arg);
void				put_message(char *message, t_philo *philo);
int					should_stop(t_philo *philo);

#endif