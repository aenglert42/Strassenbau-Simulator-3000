#include "cub3d.h"

static void	static_exit_destroy_images(void *mlx)
{
	t_img	*imgs;
	int		i;

	imgs = data()->imgs;
	i = 0;
	while (i < IMG_NBR)
	{
		if (imgs[i].ptr != NULL)
			my_destroy_image(mlx, &imgs[i]);
		i++;
	}
}

static void	static_exit_destroy_sprites(void *mlx)
{
	int			i;
	int			nbr;
	t_sprite	*sprite;

	nbr = 0;
	i = 0;
	while (nbr < SPRITE_NBR)
	{
		sprite = &data()->sprites[nbr];
		while (i <= sprite->count)
		{
			if (sprite->sequence[i].ptr != NULL)
				my_destroy_image(mlx, &sprite->sequence[i]);
			i++;
		}
		nbr++;
	}
}

void	free_all(void)
{
	void	*mlx;
	void	*win;

	config_file_destroy(&data()->config_file);
	map_destroy(&data()->map);
	mlx = data()->mlx;
	if (mlx == NULL)
		return ;
	static_exit_destroy_images(mlx);
	static_exit_destroy_sprites(mlx);
	win = data()->win;
	if (win != NULL)
		mlx_destroy_window(mlx, win);
	return ;
}
