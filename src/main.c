# include "cub3d.h"

static void static_create_3Dbackground()
{
	int		x;
	int		y;

	data()->imgs[BACKGROUND_IMG].ptr = mlx_new_image(data()->mlx, data()->window3D.width, data()->window3D.height);
	data()->imgs[BACKGROUND_IMG].addr = mlx_get_data_addr(data()->imgs[BACKGROUND_IMG].ptr, &data()->imgs[BACKGROUND_IMG].bits_per_pixel, &data()->imgs[BACKGROUND_IMG].line_length, &data()->imgs[BACKGROUND_IMG].endian);
	
	y = 0;
	while (y < data()->window3D.height)
	{
		x = 0;
		while (x < data()->window3D.width)
		{
			if (y < data()->window3D.height / 2)
				my_pixel_put(&data()->imgs[BACKGROUND_IMG], x, y, CYAN);
			else
				my_pixel_put(&data()->imgs[BACKGROUND_IMG], x, y, BLACK);
			x++;
		}
		y++;
	}
	data()->imgs[BACKGROUND_IMG].width = data()->window3D.width;
	data()->imgs[BACKGROUND_IMG].height = data()->window3D.height;
}

static void static_create_player_img()
{
	int	x;
	int y;
	data()->imgs[PLAYER_IMG].ptr = mlx_new_image(data()->mlx, PLAYER_SIZE, PLAYER_SIZE);
	data()->imgs[PLAYER_IMG].addr = mlx_get_data_addr(data()->imgs[PLAYER_IMG].ptr, &data()->imgs[PLAYER_IMG].bits_per_pixel, &data()->imgs[PLAYER_IMG].line_length, &data()->imgs[PLAYER_IMG].endian);
	
	y = 0;
	while (y < PLAYER_SIZE)
	{
		x = 0;
		while (x < PLAYER_SIZE)
		{
			my_pixel_put(&data()->imgs[PLAYER_IMG], x, y, BLACK);
			x++;
		}
		y++;
	}
	data()->imgs[PLAYER_IMG].width = PLAYER_SIZE;
	data()->imgs[PLAYER_IMG].height = PLAYER_SIZE;
}

static void	static_paint_borders(t_img *img, int img_y, int img_x)
{
	int		x;
	int		y;

	y = 0;
	while (y < GRID_SIZE)
	{
		x = 0;
		while (x < GRID_SIZE)
		{
			if ((y == 0 || y == GRID_SIZE - 1) || (x == 0 || x == GRID_SIZE -1))
				my_pixel_put(img, img_x * GRID_SIZE + x, img_y * GRID_SIZE + y, BLACK);
			x++;
		}
		y++;
	}
}

static void	static_paint_cell(t_img *img, int img_y, int img_x)
{
	int		x;
	int		y;

	y = 1;
	while (y < GRID_SIZE - 1)
	{
		x = 1;
		while (x < GRID_SIZE - 1)
		{
			if (data()->map_old[img_y][img_x] == WALL)
				my_pixel_put(img, img_x * GRID_SIZE + x, img_y * GRID_SIZE + y, GREY);
			else
				my_pixel_put(img, img_x * GRID_SIZE + x, img_y * GRID_SIZE + y, WHITE);
			x++;
		}
		y++;
	}
	static_paint_borders(img, img_y, img_x);
}

static void static_create_minimap_img()
{
	int		x;
	int		y;

	data()->imgs[MINIMAP_IMG].ptr = mlx_new_image(data()->mlx, data()->window.width, data()->window.height);
	data()->imgs[MINIMAP_IMG].addr = mlx_get_data_addr(data()->imgs[MINIMAP_IMG].ptr, &data()->imgs[MINIMAP_IMG].bits_per_pixel, &data()->imgs[MINIMAP_IMG].line_length, &data()->imgs[MINIMAP_IMG].endian);
	
	y = 0;
	while (data()->map_old[y] != NULL)
	{
		x = 0;
		while (data()->map_old[y][x] != '\0')
		{
			static_paint_cell(&data()->imgs[MINIMAP_IMG], y, x);
			x++;
		}
		y++;
	}
	data()->imgs[MINIMAP_IMG].width = data()->window.width;
	data()->imgs[MINIMAP_IMG].height = data()->window.height;
}

static void	static_initialize_data(void)
{
	data()->mlx = mlx_init();
	data()->window.width = data()->grid.width * GRID_SIZE;
	data()->window.height = data()->grid.height * GRID_SIZE;
	data()->window3D.width = WINDOW_WIDTH;
	data()->window3D.height = WINDOW_HEIGHT;
	data()->player.x = 1.5;
	data()->player.y = 1.5;
	data()->player.angle = PI / 2;
	data()->player.dx = cos(data()->player.angle) * STEP;
	data()->player.dy = sin(data()->player.angle) * STEP;
	static_create_minimap_img();
	static_create_3Dbackground();
	static_create_player_img();
}

t_data	*data(void)
{
	static t_data	data;

	return (&data);
}

int		main(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_printf("ERROR\n");
		return (EXIT_FAILURE);
	}
	read_map(argv[1]);
	static_initialize_data();
	mlx();
	return(EXIT_SUCCESS);
}