#pragma once
#include "includes.h"

#define LM_VER string("v1.2.4")

typedef HWND hwnd;
typedef HMODULE hmodule;
typedef DWORD dword;
typedef HANDLE handle;
typedef WNDPROC wndproc;
typedef UINT uint;
typedef WPARAM wparam;
typedef LPARAM lparam;
typedef RECT rect;
typedef POINT point;
typedef string str;
typedef void sample_class;
typedef void* sample_fn;

class IClientNetworkable;
typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

#define to_str to_string
#define null NULL
//#define hwid 0x0

enum weap_type : short
{
	weap_misc,
	weap_pistol,
	weap_snip,
	weap_rifle,
	weap_sub,
	weap_shot,
	weap_heavy,
	weap_max
};

inline void send_mouse(int pos_x, int pos_y, DWORD key = 0, int which_xbutton = 0)
{
	/*if (key == MOUSEEVENTF_ABSOLUTE)
	{
		GetCursorPos(&mouse);
	}*/
	INPUT input;
	MOUSEINPUT mouse_input;
	input.type = INPUT_MOUSE;
	mouse_input.dx = pos_x /*- mouse.x*/;
	mouse_input.dy = pos_y /*- mouse.y*/;
	mouse_input.mouseData = which_xbutton;
	mouse_input.dwFlags = key;//key == MOUSEEVENTF_ABSOLUTE ? MOUSEEVENTF_MOVE : key;
	mouse_input.time = 0x0;
	mouse_input.dwExtraInfo = 0x0;
	input.mi = mouse_input;
	SendInput(1, &input, sizeof(input));
	//mouse.x = 0;
	//mouse.y = 0;
}

inline void send_key(WORD key, bool on)
{
	INPUT input;
	KEYBDINPUT key_input;
	input.type = INPUT_KEYBOARD;
	key_input.wVk = key;
	key_input.wScan = 0x0;
	key_input.dwFlags = on ? 0x0 : KEYEVENTF_KEYUP;
	key_input.time = 0x0;
	key_input.dwExtraInfo = 0x0;
	input.ki = key_input;
	SendInput(1, &input, sizeof(input));
}

/*https://docs.microsoft.com/ru-ru/windows/win32/api/winuser/ns-winuser-mouseinput*/
/*inline void mouse_input(DWORD flags)
{
	INPUT input;
	MOUSEINPUT mouse_input;
	input.type = INPUT_MOUSE;
	mouse_input.dwFlags = flags;
	mouse_input.time = 0x1;
	input.mi = mouse_input;
	SendInput(1, &input, sizeof(input));
}*/

inline char change_case(char c)
{
	return std::isupper(c) ? std::tolower(c) : c;
}

// поломався
inline const char* upper_to_lower(const char* c, int size)
{
	char* sample = (char*)"";
	for (int i = 0; i < size; i++)
		sample[i] = (c[i] >= 'A' && c[i] <= 'Z') ? c[i] + ('a' - 'A') : c[i];
	return sample;
}

/*dword get_hwid()
{
	char NameBuffer[MAX_PATH];
	char SysNameBuffer[MAX_PATH];
	DWORD VSNumber;
	DWORD MCLength;
	DWORD FileSF;
	//TCHAR idp[MAX_PATH];

	GetVolumeInformationA("C:\\", NameBuffer, sizeof(NameBuffer), &VSNumber, &MCLength, &FileSF, SysNameBuffer, sizeof(SysNameBuffer));

	string strNameBuffer = NameBuffer;
	DWORD dwNameBuffer = 0x0;
	string strSysNameBuffer = SysNameBuffer;
	DWORD dwSysNameBuffer = 0x0;

	DWORD dwValue = ((VSNumber * MCLength) / FileSF);

	for (int i = 0; i < strNameBuffer.size(); i++)
	{
		dwNameBuffer += (DWORD)strNameBuffer.at(i) * i;
	}

	for (int i = 0; i < strSysNameBuffer.size(); i++)
	{
		dwSysNameBuffer += (DWORD)strSysNameBuffer.at(i) * i;
	}

	return ((dwNameBuffer * dwSysNameBuffer) * dwValue);
}

static dword p_hwid = get_hwid();*/

static const char* keynames[254] = 
{
	null, "mouse1", "mouse2", "сancel", "mouse3", "mouse4", "mouse5", null,
	"backspace", "tab", null, null, "clear", null /*это enter*/, null, null, "shift",
	"control", "alt", "pause", "caps", null, null, null, null, null, null,
	"escape", null, null, null, null, "space", "page_up", "page_down", "end", "home",
	"left", "up", "right", "down", null, null, null, "print", "insert", "delete", null,
	"but_0", "but_1", "but_2", "but_3", "but_4", "but_5", "but_6", "but_7", "but_8", "but_9", null, null, null, null, null, null, null, 
	"but_a", "but_b", "but_c", "but_d", "but_e", "but_f", "but_g", "but_h", "but_i", "but_j", "but_k", "but_l", "but_m", "but_n", 
	"but_o", "but_p", "but_q", "but_r", "but_s", "but_t", "but_u", "but_v", "but_w", "but_x", "but_y", "but_z", 
	null, null, null, null, null, "num_0", "num_1", "num_2", "num_3", "num_4", "num_5", "num_6", "num_7", "num_8", "num_9", 
	"multiply", "add", null, "subtract", "decimal", "divide",
	"f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12",
};

static char inputkeynames[254] =
{
	null, null, null, null, null, null, null, null,
	null, null, null, null, null, null /*это enter*/, null, null, null,
	null, null, null, null, null, null, null, null, null, null,
	null, null, null, null, null, ' ', null, null, null, null,
	null, null, null, null, null, null, null, null, null, null, null,
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', null, null, null, null, null, null, null,
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	null, null, null, null, null, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	null, null, null, null, null, null,
	null, null, null, null, null, null, null, null, null, null, null, null,
};