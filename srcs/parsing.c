#include "philo.h"

int	is_number(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		if (argv[i][0] == '-')
			return (0);
		if (argv[i][0] == '+')
			j++;
		while (argv[i][j])
		{
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	parce_data(char **argv, t_data *data)
{
	if (!is_number(argv))
		return (printf("Please enter numeric arguments!\n"), 0);
	data->num_of_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->num_times_to_eat = ft_atoi(argv[5]);
	else
		data->num_times_to_eat = -1;
	if (data->num_of_philos < 1 || data->num_of_philos > 200
		|| data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0 || (data->num_times_to_eat <= 0
			&& argv[5]))
	{
		return (printf("Enter a valid data!\n"), 0);
	}
	return (1);
}
