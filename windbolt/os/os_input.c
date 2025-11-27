#include "os_input.h"

local OS_InputState input_state = {0}, previous_input_state = {0};

void os_input_update(void)
{
	previous_input_state = input_state;
}

OS_InputState *os_input_get_state(void)
{
	return &input_state;
}

bool os_input_key_is_down(OS_InputKey key)
{
	return input_state.keys[key];
}

bool os_input_key_is_pressed(OS_InputKey key)
{
	return !previous_input_state.keys[key] && input_state.keys[key];
}

bool os_input_mouse_button_is_down(OS_InputMouseButton mouse_button)
{
	return input_state.mouse_buttons[mouse_button];
}

bool os_input_mouse_button_is_pressed(OS_InputMouseButton mouse_button)
{
	return !previous_input_state.mouse_buttons[mouse_button] && input_state.mouse_buttons[mouse_button];
}

void os_input_mouse_position_get(f32 *x, f32 *y)
{
	*x = input_state.mouse_x;
	*y = input_state.mouse_y;
}
