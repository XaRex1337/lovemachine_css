#pragma once
#include "definitions.h"

struct color
{
	color() { }

	color(int r, int g, int b, int a = 255)
	{
		this->r = max(0, min(r, 255));
		this->g = max(0, min(g, 255));
		this->b = max(0, min(b, 255));
		this->a = max(0, min(a, 255));
	}

	color operator+(int val)
	{
		this->r = min(255, r + val);
		this->g = min(255, g + val);
		this->b = min(255, b + val);
		return *this;
	}

	color operator-(int val)
	{
		this->r = max(0, r - val);
		this->g = max(0, g - val);
		this->b = max(0, b - val);
		return *this;
	}

	color operator+=(int val)
	{
		this->r = min(255, r + val);
		this->g = min(255, g + val);
		this->b = min(255, b + val);
		return *this;
	}

	dword to_d3d()
	{
		//return D3DCOLOR_ARGB(this->a, this->r, this->g, this->b);
		return D3DCOLOR_RGBA(this->r, this->g, this->b, this->a);
	}

	float* divide()
	{
		float divided[4] = { (float)(r / 255.f), (float)(g / 255.f), (float)(b / 255.f), (float)(a / 255.f) };
		return divided;
	}

	color with_alpha(int alpha)
	{
		return color(r, g, b, max(0, min(alpha, 255)));
	}

	color to_alpha(int alpha, int speed)
	{
		if (abs(this->a - alpha) >= speed) this->a += (alpha > a) ? speed : -speed;
		else this->a = alpha;

		return color(r, g, b, a);
	}

	// ����� ��� ����
	static color background() { return color(18, 18, 18); } // ������
	static color outline() { return color(9, 9, 9); } // ������� ���� � ������������
	static color closed_tab() { return color(30, 30, 30); } // �������� �������
	static color opened_tab() { return color(45, 45, 45); } // �������� �������
	static color text() { return color(150, 150, 150); } // �����
	static color ptext() { return color(200, 200, 200); } // ��������� �����
	static color disabled() { return color(40, 40, 40); } // ������� ��������
	static color enabled() { return color(152, 26, 152); } // ������� �������

	// ��� ������� ����
	static color lm() { return color(49, 124, 230); }

	int r, g, b, a;
};