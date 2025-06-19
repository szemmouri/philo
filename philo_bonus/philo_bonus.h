/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szemmour <szemmour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:37:49 by szemmour          #+#    #+#             */
/*   Updated: 2025/06/19 10:09:20 by szemmour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>

# define FORK_SEM "/forks_sem"
# define WRITE_SEM "/write_sem"
# define LOCK_SEM "/lock_sem"

typedef struct s_data
{
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	int		num_of_philos;
	int		num_times_to_eat;
	size_t	start_time;
	sem_t	*forks;
	sem_t	*write_sem;
	sem_t	*lock_sem;
}			t_data;

typedef struct s_philo
{
	pid_t	pid;
	int		id;
	int		meals_eaten;
	size_t	last_meal;
	t_data	*data;
}			t_philo;

int			parce_data(char **argv, t_data *data);
int			ft_atoi(const char *str);
size_t		get_current_time(void);
int			ft_usleep(size_t milliseconds);
void		ft_putstr(char *str);
void		ft_clean(t_philo *philos);
void		start_sem(t_philo *philos);
void		put_message(char *message, t_philo *philo);
void		wait_children(t_philo *philos);

#endif