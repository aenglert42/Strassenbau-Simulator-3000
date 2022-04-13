# include "cub3d.h"

static t_ray	static_fill_ray_struct(t_ray ray, int index)
{
	ray.index = index;
	ray.len = sqrt((ray.x - data()->player.x) * (ray.x - data()->player.x) + (ray.y - data()->player.y) * (ray.y - data()->player.y));
	float	delta;
	delta = data()->player.angle - ray.angle;
	if (delta < 0)
		delta += 2 * PI;
	if (delta > 2 * PI)
		delta -= 2 * PI;
	ray.dist = ray.len * cos(delta);
	return (ray);
}

static void		static_iterate(t_ray *ray)
{
	int i = 0;
	while (i < DEPTH_OF_FIELD)
	{
		if ((*ray).y >= 0 && (*ray).x >= 0 && (*ray).y < data()->map.height && (*ray).x < data()->map.width && data()->map.grid[(int)(*ray).y][(int)(*ray).x] == WALL)
			break ;
		else
		{
			(*ray).x += (*ray).dx;
			(*ray).y += (*ray).dy;
			i++;
		}
	}
}

static t_ray	static_calculate_ray_v(float angle, int index)
{
	t_ray	ray;
	float	nTan;

	ray.angle = angle;
	nTan = -tan(ray.angle);
	if (ray.angle > PI1 && ray.angle < PI3) //looking left
	{
		ray.orientation = WEST;
		ray.x = (float)trunc(data()->player.x) - EDGE;
		ray.y = (data()->player.x - ray.x) * nTan + data()->player.y;
		ray.dx = -1;
		ray.dy = -ray.dx * nTan;
		static_iterate(&ray);
	}
	else if (ray.angle < PI1 || ray.angle > PI3) //looking right
	{
		ray.orientation = EAST;
		ray.x = (float)ceil(data()->player.x);
		ray.y = (data()->player.x - ray.x) * nTan + data()->player.y;
		ray.dx = 1;
		ray.dy = -ray.dx * nTan;
		static_iterate(&ray);
	}
	else// (ray.angle == 0 || ray.angle == PI) //looking vertical
	{
		ray.x = data()->player.x;
		ray.y = data()->player.y;
	}
	return (static_fill_ray_struct(ray, index));
}

static t_ray	static_calcualte_ray_h(float angle, int index)
{
	t_ray	ray;
	float	aTan;
	
	ray.angle = angle;
	aTan = -1 / tan(ray.angle);
	if (ray.angle > PI) //looking up
	{
		ray.orientation = NORTH;
		ray.y = (float)trunc(data()->player.y) - EDGE;
		ray.x = (data()->player.y - ray.y) * aTan + data()->player.x;
		ray.dy = -1;
		ray.dx = -ray.dy * aTan;
		static_iterate(&ray);
	}
	else if (ray.angle < PI) //looking down
	{
		ray.orientation = SOUTH;
		ray.y = (float)ceil(data()->player.y);
		ray.x = (data()->player.y - ray.y) * aTan + data()->player.x;
		ray.dy = 1;
		ray.dx = -ray.dy * aTan;
		static_iterate(&ray);
	}
	else// (ray.angle == 0 || ray.angle == PI) //looking sideways
	{
		ray.x = data()->player.x;
		ray.y = data()->player.y;
	}
	return (static_fill_ray_struct(ray, index));
}

static t_ray	static_ray_calculate(float angle, int index)
{
	
	t_ray	rays[2];
	t_ray	ray;
	// t_point	p;
	// t_point	a;

	rays[0] = static_calcualte_ray_h(angle, index);
	rays[1] = static_calculate_ray_v(angle, index);
	if (rays[0].len < rays[1].len)
		ray = rays[0];
	else
		ray = rays[1];
	// if (index != 0 && index != NUMBER_OF_RAYS -1)
	// 	return (ray);
	// if (ray.y >= 0 && ray.x >= 0 && ray.y < data()->map.height && ray.x < data()->map.width)
	// {
	// 	p.x = ray.x * GRID_SIZE;
	// 	p.y = ray.y * GRID_SIZE;
	// 	a.x = data()->player.x * GRID_SIZE;
	// 	a.y = data()->player.y * GRID_SIZE;
	// 	draw_line_a_to_b(img, a, p, RED);
	// }
	return (ray);
}

void	rays_draw_to_image(void)
{
	int		i;
	t_data	*datas;
	t_img	*img;
	t_point	p;
	t_point	a;

	datas = data();
	img = &datas->imgs[RAYS_IMG];
	img->ptr = mlx_new_image_alpha(datas->mlx, datas->minimap.width, datas->minimap.height);
	if (img->ptr == NULL)
		exit_program(MLX_IMAGE);
	img->addr = mlx_get_data_addr(img->ptr, &img->bits_per_pixel, &img->line_length, &img->endian);
	i = 0;
	while (i < NUMBER_OF_RAYS)
	{
		if (!(i % MINIMAP_RAY_DENSITY_FACTOR))
		{
			if (datas->rays[i].y >= 0 && datas->rays[i].x >= 0 && datas->rays[i].y < data()->map.height && datas->rays[i].x < data()->map.width)
			{
				p.x = datas->rays[i].x * GRID_SIZE;
				p.y = datas->rays[i].y * GRID_SIZE;
				a.x = data()->player.x * GRID_SIZE;
				a.y = data()->player.y * GRID_SIZE;
				draw_line_a_to_b(img, a, p, RED);
			}
		}
		i++;
	}
}

void	rays_calculate(void)
{
	int		i;
	float	angle;
	t_data	*datas;

	datas = data();
	i = 0;
	angle = datas->player.angle - ANGLE_OF_VIEW / 2 * DR;
	if (angle < 0)
		angle += 2 * PI;
	if (angle > 2 * PI)
		angle -= 2 * PI;
	while (i < NUMBER_OF_RAYS)
	{
		datas->rays[i] = static_ray_calculate(angle, i);
		datas->rays[i].lineH = (datas->window.height / datas->rays[i].dist) * datas->window.width / datas->window.height * 60 / ANGLE_OF_VIEW;
		angle += ANGLE_OF_VIEW * DR / NUMBER_OF_RAYS;
		if (angle < 0)
			angle += 2 * PI;
		if (angle > 2 * PI)
			angle -= 2 * PI;
		i++;
	}
}