#include "philo.h"

int	ft_atoi(const char *str)
{
	int					i;
	int					sign;
	unsigned long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign == 1 && result > 9223372036854775807)
			return (-1);
		if (sign == -1 && result > 9223372036854775807)
			return (0);
		result = (result * 10) + (str[i] - 48);
		i++;
	}
	return ((int)(result * sign));
}

size_t	get_time(void)
{
	struct timeval tv;
	size_t time_in_ms;

	gettimeofday(&tv, NULL);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}