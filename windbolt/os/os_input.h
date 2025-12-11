#ifndef OS_INPUT_H
#define OS_INPUT_H

#include "types.h"

typedef enum
{
    OS_KEY_BACKSPACE = 0x08,
    OS_KEY_ENTER = 0x0D,
    OS_KEY_TAB = 0x09,
    OS_KEY_SHIFT = 0x10,
    OS_KEY_CONTROL = 0x11,
    
    OS_KEY_PAUSE = 0x13,
    OS_KEY_CAPITAL = 0x14,
    
    OS_KEY_ESCAPE = 0x1B,
    
    OS_KEY_CONVERT = 0x1C,
    OS_KEY_NONCONVERT = 0x1D,
    OS_KEY_ACCEPT = 0x1E,
    OS_KEY_MODECHANGE = 0x1F,
    
    OS_KEY_SPACE = 0x20,
    OS_KEY_PAGE_UP = 0x21,
    OS_KEY_PAGE_DOWN = 0x22,
    OS_KEY_END = 0x23,
    OS_KEY_HOME = 0x24,
    OS_KEY_LEFT = 0x25,
    OS_KEY_UP = 0x26,
    OS_KEY_RIGHT = 0x27,
    OS_KEY_DOWN = 0x28,
    OS_KEY_SELECT = 0x29,
    OS_KEY_PRINT = 0x2A,
    OS_KEY_EXECUTE = 0x2B,
    OS_KEY_PRINT_SCREEN = 0x2C,
    OS_KEY_INSERT = 0x2D,
    OS_KEY_DELETE = 0x2E,
    OS_KEY_HELP = 0x2F,
    
    OS_KEY_0 = 0x30,
    OS_KEY_1 = 0x31,
    OS_KEY_2 = 0x32,
    OS_KEY_3 = 0x33,
    OS_KEY_4 = 0x34,
    OS_KEY_5 = 0x35,
    OS_KEY_6 = 0x36,
    OS_KEY_7 = 0x37,
    OS_KEY_8 = 0x38,
    OS_KEY_9 = 0x39,
    
    OS_KEY_A = 0x41,
    OS_KEY_B = 0x42,
    OS_KEY_C = 0x43,
    OS_KEY_D = 0x44,
    OS_KEY_E = 0x45,
    OS_KEY_F = 0x46,
    OS_KEY_G = 0x47,
    OS_KEY_H = 0x48,
    OS_KEY_I = 0x49,
    OS_KEY_J = 0x4A,
    OS_KEY_K = 0x4B,
    OS_KEY_L = 0x4C,
    OS_KEY_M = 0x4D,
    OS_KEY_N = 0x4E,
    OS_KEY_O = 0x4F,
    OS_KEY_P = 0x50,
    OS_KEY_Q = 0x51,
    OS_KEY_R = 0x52,
    OS_KEY_S = 0x53,
    OS_KEY_T = 0x54,
    OS_KEY_U = 0x55,
    OS_KEY_V = 0x56,
    OS_KEY_W = 0x57,
    OS_KEY_X = 0x58,
    OS_KEY_Y = 0x59,
    OS_KEY_Z = 0x5A,
    
    OS_KEY_LSUPER = 0x5B,
    OS_KEY_RSUPER = 0x5C,
    OS_KEY_APPS = 0x5D,
    
    OS_KEY_SLEEP = 0x5F,
    
    OS_KEY_NUMPAD0 = 0x60,
    OS_KEY_NUMPAD1 = 0x61,
    OS_KEY_NUMPAD2 = 0x62,
    OS_KEY_NUMPAD3 = 0x63,
    OS_KEY_NUMPAD4 = 0x64,
    OS_KEY_NUMPAD5 = 0x65,
    OS_KEY_NUMPAD6 = 0x66,
    OS_KEY_NUMPAD7 = 0x67,
    OS_KEY_NUMPAD8 = 0x68,
    OS_KEY_NUMPAD9 = 0x69,
    OS_KEY_MULTIPLY = 0x6A,
    OS_KEY_ADD = 0x6B,
    OS_KEY_SEPARATOR = 0x6C,
    OS_KEY_SUBTRACT = 0x6D,
    OS_KEY_DECIMAL = 0x6E,
    OS_KEY_DIVIDE = 0x6F,
    
    OS_KEY_F1 = 0x70,
    OS_KEY_F2 = 0x71,
    OS_KEY_F3 = 0x72,
    OS_KEY_F4 = 0x73,
    OS_KEY_F5 = 0x74,
    OS_KEY_F6 = 0x75,
    OS_KEY_F7 = 0x76,
    OS_KEY_F8 = 0x77,
    OS_KEY_F9 = 0x78,
    OS_KEY_F10 = 0x79,
    OS_KEY_F11 = 0x7A,
    OS_KEY_F12 = 0x7B,
    OS_KEY_F13 = 0x7C,
    OS_KEY_F14 = 0x7D,
    OS_KEY_F15 = 0x7E,
    OS_KEY_F16 = 0x7F,
    OS_KEY_F17 = 0x80,
    OS_KEY_F18 = 0x81,
    OS_KEY_F19 = 0x82,
    OS_KEY_F20 = 0x83,
    OS_KEY_F21 = 0x84,
    OS_KEY_F22 = 0x85,
    OS_KEY_F23 = 0x86,
    OS_KEY_F24 = 0x87,
    
    OS_KEY_NUMLOCK = 0x90,
    
    OS_KEY_SCROLL = 0x91,
    
    OS_KEY_NUMPAD_EQUAL = 0x92,
    
    OS_KEY_LSHIFT = 0xA0,
    OS_KEY_RSHIFT = 0xA1,
    OS_KEY_LCONTROL = 0xA2,
    OS_KEY_RCONTROL = 0xA3,
    OS_KEY_LALT = 0xA4,
    OS_KEY_RALT = 0xA5,
    
    OS_KEY_SEMICOLON = 0x3B,
    
    OS_KEY_APOSTROPHE = 0xDE,
    OS_KEY_QUOTE = OS_KEY_APOSTROPHE,
    OS_KEY_EQUAL = 0xBB,
    OS_KEY_COMMA = 0xBC,
    OS_KEY_MINUS = 0xBD,
    OS_KEY_PERIOD = 0xBE,
    OS_KEY_SLASH = 0xBF,
    
    OS_KEY_GRAVE = 0xC0,
    
    OS_KEY_LBRACKET = 0xDB,
    OS_KEY_PIPE = 0xDC,
    OS_KEY_BACKSLASH = OS_KEY_PIPE,
    OS_KEY_RBRACKET = 0xDD,
    
    OS_MAX_KEYS = 0xFF
} OS_InputKey;

typedef enum
{
    OS_MOUSE_BUTTON_LEFT,
    OS_MOUSE_BUTTON_RIGHT,
    OS_MOUSE_BUTTON_MIDDLE,
    
    OS_MAX_MOUSE_BUTTONS
} OS_InputMouseButton;

typedef struct
{
    OS_InputKey keys[OS_MAX_KEYS];
    OS_InputMouseButton mouse_buttons[OS_MAX_MOUSE_BUTTONS];
    f32 mouse_x, mouse_y;
} OS_InputState;

// NOTE(smoke): copies input_state to previous_input_state,
// therefore must be called at the end of the frame
global void os_input_update(void);

// NOTE(smoke): this if for the platform backend
global OS_InputState *os_input_get_state(void);

global bool os_input_key_is_down(OS_InputKey key);
global bool os_input_key_is_pressed(OS_InputKey key);
global bool os_input_mouse_button_is_down(OS_InputMouseButton mouse_button);
global bool os_input_mouse_button_is_pressed(OS_InputMouseButton mouse_button);
global void os_input_mouse_position_get(f32 *x, f32 *y);

#endif // OS_INPUT_H
