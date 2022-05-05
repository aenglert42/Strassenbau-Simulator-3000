#include "cub3d.h"

int	get_sky_color(t_ray *ray, int y)
{
	int	tx;
	int	ty;

	if (!SKY_ENABLED)
		return (data()->map.ceiling.rgb);
	tx = ray->angle * (data()->imgs[SKY_IMG].width / (2 * M_PI));
	ty = y + data()->imgs[SKY_IMG].height / 4 - data()->player.dz;
	return (get_pixel_color(&data()->imgs[SKY_IMG], tx, ty));
}

static float	static_get_floor_value(float player_value,
	float ray_value, int y, float wall_dist)
{
	float	current_dist;
	float	weight;
	float	current_floor_value;

	current_dist = fabs(data()->window.height / (2.0 * (y - data()->player.dz)
				- data()->window.height - 1));
	weight = current_dist / wall_dist;
	current_floor_value = weight * (ray_value - player_value) * 2
		+ player_value;
	// return ((current_floor_value - (int)current_floor_value) * TEXTURE_SIZE);
	return (current_floor_value);
}

static int	static_get_texture_color(int image, t_ray *ray, int y)
{
	float	map_x;
	float	map_y;
	int		tx;
	int		ty;

	map_x = static_get_floor_value(data()->player.x,
			ray->x, y, ray->dist);
	map_y = static_get_floor_value(data()->player.y,
			ray->y, y, ray->dist);
	tx = (map_x - (int)map_x) * TEXTURE_SIZE;
	ty = (map_y - (int)map_y) * TEXTURE_SIZE;
	return (get_pixel_color(&data()->imgs[image], tx, ty));
}

int	get_ceiling_color(t_ray *ray, int y)
{
	int	image;
	
	image = CEILING_IMG;
	if (CEILING_TEXTURE_ENABLED)
		return (static_get_texture_color(image, ray, y));
	else if (SKY_ENABLED)
		return (get_sky_color(ray, y));
	return (data()->map.ceiling.rgb);
}

// static t_point static_get_coordiante(float player_value,
// 	float ray_value, int y, float wall_dist)
// {
// 	t_point	p;

// 	p.x = 0;
// 	p.y = 0;
// 	float	current_dist;
// 	float	weight;
// 	float	current_floor_value;

// 	current_dist = fabs(data()->window.height / (2.0 * (y - data()->player.dz)
// 				- data()->window.height));
// 	weight = current_dist / wall_dist;
// 	current_floor_value = weight * (ray_value - player_value)
// 		+ player_value / 2;
// 	return (p);
// }

static int	static_get_texture(t_ray *ray, int y)
{
	int	map_x;
	int	map_y;

	map_x = static_get_floor_value(data()->player.x,
			ray->x, y, ray->dist);
	map_y = static_get_floor_value(data()->player.y,
			ray->y, y, ray->dist);
	if (map_x >= (int)data()->map.width || map_y >= (int)data()->map.height) //better check
		return (FLOOR_IMG);
	// if (ray->index == 59) //remove
	// 	printf("ray%d rx: %f ry: %f infinite: %d map_x: %d, map_y: %d c: %c\n", ray->index, ray->x, ray->y, ray->is_infinite, map_x, map_y, data()->map.grid[map_y][map_x]); //remove
	if (data()->map.grid[map_y][map_x] == VOID)
		return (SKY_IMG);
	return (FLOOR_IMG);
}

int	get_floor_color(t_ray *ray, int y)
{
	int	image;

	image = static_get_texture(ray, y);
	if (image == SKY_IMG) //better solution
		return (get_sky_color(ray, y));
	if (FLOOR_TEXTURE_ENABLED)
		return (static_get_texture_color(image, ray, y));
	return (data()->map.floor.rgb);
}
