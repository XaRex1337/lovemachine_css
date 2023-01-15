#pragma once
#include "includes.h"
#include "definitions.h"
#include "color.h"

//#define DEBUG_LOG

ofstream myfile;

// TODO: сделать это дерьмо юзабельным
namespace console
{
	// тип консоли
	enum console_type : int
	{
		window,
		con_menu
	};
	int type = window;

	// цвета // TODO: добавить адаптацию цветов под ingame консоль
	#define darkgreen 2
	#define darkred 12
	#define darkwhite 7
	#define green 10
	#define red 4
	const int defcolor = 11;

	struct console_line
	{
		console_line(string text, color ccolor)
		{
			this->text = text;
			this->ccolor = ccolor;
		}
		
		string text;
		color ccolor;
	};

	string cur_text = str("no text");
	color cur_color;
	deque<console_line> lines;

	handle hconsole, herror;

	void set_color(int color = -1)
	{
		if (color == -1)
		{
			SetConsoleTextAttribute(hconsole, defcolor);
		}
		else
		{
			SetConsoleTextAttribute(hconsole, color);
		}
	}

	void write(string text, int color = -1)
	{
		set_color(color);
		cout << text << endl;
		myfile << text << endl;
	}

	void write_hex(string text, DWORD val, int color = -1)
	{
		set_color(color);
		cout << text << " : " << "0x" << val << endl;
		myfile << text << " : " << "0x" << val << endl;
		cout << dec;
		myfile << dec;
	}

	void add_line()
	{
		lines.push_back(console_line(cur_text, cur_color));
	}

	void write(string text, color ccolor, int ctype = con_menu)
	{
		cur_text = text;
		cur_color = ccolor;
		dword func = (dword)add_line;
		_asm call add_line
	}

	void create(string name)
	{
		if (type == window)
		{
			AllocConsole(); // вводим консоль
			freopen("CONIN$", "r", stdin); // ввод
			freopen("CONOUT$", "w", stdout); // вывод
			freopen("CONOUT$", "w", stderr); // вывод ошибок

			SetConsoleTitleA(name.c_str()); // установка имени

			hconsole = GetStdHandle(STD_OUTPUT_HANDLE); // хендл потока вывода
			herror = GetStdHandle(STD_ERROR_HANDLE); // хендл потока ошибок

			SetConsoleTextAttribute(hconsole, defcolor);
			SetConsoleTextAttribute(herror, red);

			write("lovemachine injected\nconsole created");
		}
	}

	void remove()
	{
		if (hconsole == NULL || herror == NULL) return;

		console::write("removing console");
		FreeConsole();
	}
}