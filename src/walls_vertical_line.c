/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls_vertical_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 11:55:14 by coder             #+#    #+#             */
/*   Updated: 2022/05/16 09:45:07 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	static_walls_determine_texture_image(t_ray *ray)
{
	if (ray->orientation == NORTH)
		return (NORTH_IMG);
	else if (ray->orientation == SOUTH)
		return (SOUTH_IMG);
	else if (ray->orientation == EAST)
		return (EAST_IMG);
	else
		return (WEST_IMG);
}

static int	static_walls_determine_tx(t_ray *ray)
{
	int		tx;

	if (ray->orientation == NORTH || ray->orientation == SOUTH)
		tx = (ray->x - (int)ray->x) * TEXTURE_SIZE;
	else
		tx = (ray->y - (int)ray->y) * TEXTURE_SIZE;
	if (ray->orientation == WEST
		|| ray->orientation == SOUTH)
		tx = TEXTURE_SIZE - 1 - tx;
	if (tx < 0)
		tx = 0;
	return (tx);
}

static int	static_get_wall_color(t_ray *ray, int y)
{
	int		tx;
	float	ty;
	int		image;

	image = static_walls_determine_texture_image(ray);
	tx = static_walls_determine_tx(ray);
	ty = ray->tyoffset
		+ (TEXTURE_SIZE / ray->line_h) * (y - ray->start_y);
	if (ty > data()->window.height - 1)
		ty = data()->window.height - 1;
	return (get_pixel_color(&data()->imgs[image], tx, ty));
}

static void	static_walls_draw_single_line(t_data *data, t_img *img, t_ray *ray)
{
	int		y;
	int		color;

	y = 0;
	while (y < data->window.height)
	{
		if (y < ray->start_y)
			color = get_ceiling_color(ray, y);
		else if (y <= ray->end_y + 1)
		{
			if (ray->is_infinite)
				color = get_sky_color(ray, y);
			else
				color = static_get_wall_color(ray, y);
		}
		else if (y < data->window.height)
			color = get_floor_color(ray, y);
		my_pixel_put(img, ray->screen_x, y, color);
		y++;
	}
}

void	walls_draw_segment(t_data *data, t_img *img, t_ray *ray)
{
	int		line_i;

	line_i = 0;
	while (line_i < data->line_w)
	{
		ray->screen_x = data->line_w * ray->index + line_i;
		if (ray->screen_x < 0)
			ray->screen_x = 0;
		if (ray->screen_x > data->window.width - 1)
			ray->screen_x = data->window.width - 1;
		static_walls_draw_single_line(data, img, ray);
		line_i++;
	}
}
