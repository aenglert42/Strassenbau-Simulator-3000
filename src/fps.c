# include "cub3d.h"
#include <sys/time.h>
#include <time.h>

static long	static_get_current_time(void)
{
	long			current_time;
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	current_time = timeval.tv_sec * 1000 + timeval.tv_usec / 1000;
	return (current_time);
}

void	timedifference(char *str)
{
	long	currentTime;
	long	delta;

	currentTime = clock();
	delta = currentTime - data()->time;
	data()->time = currentTime;
	// if (ft_strcmp(str, "START") || ft_strcmp(str, "END"))
		printf("%s time:\t%lu clocks\n", str, delta);
}

void	fps_to_window_buffer(void)
{
	long	currentTime;
	long	elapse_time;
	int		fps;
	char	*str;

	if (data()->fps.first)
	{
		data()->fps.lastTime = static_get_current_time();
		data()->fps.first = false;
	}
	currentTime = static_get_current_time();
	elapse_time = currentTime - data()->fps.lastTime;
	fps = 1000.0/elapse_time;
	data()->fps.lastTime = currentTime;
	str = ft_itoa(fps);
	mlx_string_put(data()->mlx, data()->win, 20, 500, ORANGE, str); //remove
	mlx_string_put(data()->mlx, data()->win, 40, 500, ORANGE, "FPS"); //remove
	free(str);
}