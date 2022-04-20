#include "cub3d.h"

static void	static_new_minimap(t_img *dst)
{
	t_point	dst_p;
	int		color;
	float	map_x;
	float	map_y;

	map_y = (data()->player.y - MICROMAP_RADIUS);
	dst_p.y = 0;
	while (dst_p.y < dst->height)
	{
		dst_p.x = 0;
		map_x = (data()->player.x - MICROMAP_RADIUS);
		while (dst_p.x < dst->width)
		{
			color = MINIMAP_BACKGROUND_COLOR;
			// printf("map_y: %f, map_x: %f\n", map_y, map_x);
			if (map_y < data()->map.height && map_y >= 0 && map_x < data()->map.width
		&& map_x >= 0) //inside map function
			{
				if (data()->map.grid[(int)map_y][(int)map_x] == WALL)
					color = MINIMAP_WALL_COLOR;
				else
					color = MINIMAP_SPACE_COLOR;
				if ((map_y > (int)map_y - EPSILON && map_y < (int)map_y + EPSILON) || (map_x > (int)map_x - EPSILON && map_x < (int)map_x + EPSILON))
					color = MINIMAP_GRID_COLOR;
			}
			my_pixel_put(dst, dst_p.x, dst_p.y, color);
			dst_p.x++;
			map_x = map_x + 1.0 / GRID_SIZE;
		}
		dst_p.y++;
		map_y = map_y + 1.0 / GRID_SIZE;
	}
}

// static void	static_copy_img_section(t_img *src, t_img *dst)
// {
// 	t_point	dst_p;
// 	t_point	src_p;
// 	int		color;

// 	src_p.y = (data()->player.y - MICROMAP_RADIUS) * GRID_SIZE;
// 	src_p.x = (data()->player.x - MICROMAP_RADIUS) * GRID_SIZE;
// 	dst_p.y = 0;
// 	while (dst_p.y < dst->height)
// 	{
// 		dst_p.x = 0;
// 		while (dst_p.x < dst->width)
// 		{
// 			color = MINIMAP_BACKGROUND_COLOR;
// 			if (is_inside_limits(src_p.x + dst_p.x, src_p.y + dst_p.y, src))
// 			{
// 				color = *(unsigned int *)(src->addr
// 						+ (unsigned int)((int)(src_p.y + dst_p.y)*src->line_len
// 							+ (src_p.x + dst_p.x) * src->bits_per_pixel / 8));
// 			}
// 			if (color != TRANSPARENT)
// 				my_pixel_put(dst, dst_p.x, dst_p.y, color);
// 			dst_p.x++;
// 		}
// 		dst_p.y++;
// 	}
// }

void	micromap_draw_to_image(void)
{
	t_img	*img;

	img = &data()->imgs[MICROMAP_IMG];
	img->ptr = my_new_image(data()->mlx, MICROMAP_RADIUS * 2 * GRID_SIZE,
			MICROMAP_RADIUS * 2 * GRID_SIZE, img);
	if (img->ptr == NULL)
		exit_program(MLX_IMAGE);
	static_new_minimap(img);
	// static_copy_img_section(&data()->imgs[MINIMAP_IMG], img);
	// static_copy_img_section(&data()->imgs[RAYS_IMG], img);
}
