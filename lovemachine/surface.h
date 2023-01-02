#pragma once
#include "game shit.h"

namespace surf
{
	namespace font
	{
		enum e_font : int
		{
			esp,
			max_fonts
		};

		unsigned long fonts[max_fonts];

		void setup(e_font font_id, const char* name, int tall, int weight, int flags)
		{
			fonts[font_id] = _surface->create_font();
			if (!fonts[font_id])
				console::write("/surface/ failed to create fonts[" + to_str(font_id) + "]", red);
			
			if (!_surface->set_font_glyph_set(fonts[font_id], name, tall, weight, null, null, flags))
				console::write("/surface/ failed to set font glyph set for fonts[" + to_str(font_id) + "]", red);
			
			console::write_hex("/surface/ fonts[" + to_str(font_id) + "]", fonts[font_id], green);
		}

		void size(e_font font_id, int& width, int& height, const char* text, ...)
		{
			char buf[1024] = { '\0' };
			va_list va_alist;
			va_start(va_alist, text);
			int length = vsprintf_s(buf, text, va_alist);
			va_end(va_alist);

			wchar_t* wstr = new wchar_t[length + 1];
			mbstowcs(wstr, buf, length + 1);

			_surface->get_text_size(fonts[font_id], wstr, width, height);

			delete wstr;
		}

		void draw(e_font font_id, int x, int y, color p_color, DWORD flag, const char* fmt, ...)
		{
			char buf[1024] = { '\0' };
			va_list va_alist;
			va_start(va_alist, fmt);
			vsprintf_s(buf, fmt, va_alist);
			va_end(va_alist);

			//wchar_t* wstr = new wchar_t[length + 1];
			//mbstowcs(wstr, buf, length + 1);

			int wchars_num = MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
			wchar_t* wstr = new wchar_t[wchars_num];
			MultiByteToWideChar(CP_UTF8, 0, buf, -1, wstr, wchars_num);

			int p_x = x, p_y = y;

			if (flag != null)
			{
				int sx, sy; _surface->get_text_size(fonts[font_id], wstr, sx, sy);
				if (flag & DT_CENTER) p_x -= sx / 2;
				if (flag & DT_VCENTER)p_y -= sy / 2;
				if (flag & DT_RIGHT)  p_x -= sx;
				if (flag & DT_BOTTOM) p_y -= sy;
			}

			_surface->set_text_color(p_color.r, p_color.g, p_color.b, p_color.a);
			_surface->set_font(fonts[font_id]);
			_surface->set_text_pos(p_x, p_y);
			_surface->print_text(wstr, wcslen(wstr));
		
			delete wstr;
		}
	}

	namespace prim
	{
		void filled_box(int x, int y, int x1, int y1, color p_color)
		{
			_surface->set_color(p_color.r, p_color.g, p_color.b, p_color.a);
			_surface->filled_rect(x, y, x1, y1);
		}

		void bordered_box(int x, int y, int x1, int y1, color p_color)
		{
			_surface->set_color(p_color.r, p_color.g, p_color.b, p_color.a);
			_surface->outlined_rect(x, y, x1, y1);
		}

		void line(int x, int y, int x1, int y1, color p_color)
		{
			_surface->set_color(p_color.r, p_color.g, p_color.b, p_color.a);
			_surface->line(x, y, x1, y1);
		}
	}
}