# include "cub3d.h"

static int	static_key_release_hook(int keycode, t_keys *keys)
{
	if (keycode == W_KEY || keycode == UP_KEY)
		keys->forwards = false;
	else if (keycode == A_KEY || keycode == LEFT_KEY)
		keys->turnleft = false;
	else if (keycode == S_KEY || keycode == DOWN_KEY)
		keys->turnright = false;
	else if (keycode == D_KEY || keycode == RIGHT_KEY)
		keys->backwards = false;
	return (EXIT_SUCCESS);
}

static int	static_key_press_hook(int keycode, t_keys *keys)
{
	if (keycode == ESC_KEY)
		exit_program(SUCCESS);
	if (keycode == W_KEY || keycode == UP_KEY)
		keys->forwards = true;
	else if (keycode == A_KEY || keycode == LEFT_KEY)
		keys->turnleft = true;
	else if (keycode == S_KEY || keycode == DOWN_KEY)
		keys->turnright = true;
	else if (keycode == D_KEY || keycode == RIGHT_KEY)
		keys->backwards = true;
	return (EXIT_SUCCESS);
}

void	mlx(void)
{
	t_keys keys;

	keys.backwards = false;
	keys.forwards = false;
	keys.turnleft = false;
	keys.turnright = false;
	
	data()->win = mlx_new_window(data()->mlx, data()->window.width,
		data()->window.height, "cub3D");
	if (data()->win == NULL)
		exit_program(MLX_WIN);
	mlx_hook(data()->win, DestroyNotify,
		StructureNotifyMask, exit_program, NULL); //why isn't this a problem? (NULL)
	mlx_hook(data()->win, KeyPress, KeyPressMask, static_key_press_hook, &keys);
	mlx_hook(data()->win, KeyRelease, KeyReleaseMask, static_key_release_hook, &keys);
	mlx_loop_hook(data()->mlx, game, &keys);
	mlx_loop(data()->mlx);
}
