# include "cub3d.h"
#include <sys/time.h>

static long	static_get_current_time(void)
{
	long			current_time;
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	current_time = timeval.tv_sec * 1000 + timeval.tv_usec / 1000;
	return (current_time);
}

char	*get_fps(void)
{
	long	currentTime;
	long	elapse_time;
	int		fps;

	if (data()->fps.first)
	{
		data()->fps.lastTime = static_get_current_time();
		data()->fps.first = false;
	}
	currentTime = static_get_current_time();
	elapse_time = currentTime - data()->fps.lastTime;
	fps = 1000.0/elapse_time;
	data()->fps.lastTime = currentTime;
	return (ft_itoa(fps));
}