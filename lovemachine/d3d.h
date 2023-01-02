#pragma once
#include "includes.h"
#include "definitions.h"
#include "console.h"
#include "color.h"

namespace d3d
{
	IDirect3DDevice9* device;

	namespace font
	{
		enum e_font : int
		{
			tab,
			cont,
			hitmarker_big,
			hitmarker_small,
			esp,
			max_fonts
		};

		struct font_info
		{
			font_info() { }

			font_info(const char* name, dword scale, dword flags)
			{
				this->name = name;
				this->scale = scale;
				this->flags = flags;
			}

			const char* name;
			dword scale, flags;
		};

		font_info fonts_info[max_fonts];
		LPD3DXFONT fonts[max_fonts];

		void setup(e_font font_id, const char* name, dword scale, dword flags = NULL)
		{
			fonts_info[font_id] = font_info(name, scale, flags);
		}

		long reset()
		{
			for (int font_id = 0; font_id < max_fonts; font_id++)
			{
				while (!fonts[font_id])
				{
					D3DXCreateFontA(device, fonts_info[font_id].scale, 0/*ширину определит сам*/, fonts_info[0].flags, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fonts_info[font_id].name, &fonts[font_id]);
				}

				console::write_hex("/d3d/ fonts[" + to_str(font_id) + "]", (dword)fonts[font_id], green);
			}

			return S_OK;
		}

		long restore()
		{
			for (int font_id = 0; font_id < max_fonts; font_id++)
			{
				if (!fonts[font_id])
					continue;

				fonts[font_id]->Release();
				fonts[font_id] = NULL;
			}

			return S_OK;
		}

		inline rect size(e_font font_id, const char* text, ...)
		{
			char buf[1024] = { '\0' };
			va_list va_alist;
			va_start(va_alist, text);
			vsprintf_s(buf, text, va_alist);
			va_end(va_alist);

			rect size = { 0, 0, 0, 0 };
			fonts[font_id]->DrawTextA(0, buf, strlen(buf), &size, DT_CALCRECT, D3DCOLOR_RGBA(0, 0, 0, 255));

			auto width = size.right - size.left;
			auto height = size.bottom - size.top;

			return { width, height, width, height };
		}

		inline rect size_w(e_font font_id, const wchar_t* text, int wchars_num = -1)
		{
			rect size = { 0, 0, 0, 0 };
			fonts[font_id]->DrawTextW(0, text, wchars_num == -1 ? wcslen(text) : wchars_num, &size, DT_CALCRECT, D3DCOLOR_RGBA(0, 0, 0, 255));

			auto width = size.right - size.left;
			auto height = size.bottom - size.top;

			return { width, height, width, height };
		}

		void draw(e_font font_id, int x, int y, color p_color, DWORD flag, const char* fmt, ...)
		{
			char buf[1024] = { '\0' };
			va_list va_alist;
			va_start(va_alist, fmt);
			vsprintf_s(buf, fmt, va_alist);
			va_end(va_alist);

			rect position = { x, y, 0, 0 };

			if (flag != null)
			{
				rect tsize = size(font_id, buf);
				if (flag & DT_CENTER) position.left -= tsize.left / 2;
				if (flag & DT_VCENTER)position.top -= tsize.top / 2;
				if (flag & DT_RIGHT)  position.left -= tsize.left;
				if (flag & DT_BOTTOM) position.top -= tsize.top;
			}

			fonts[font_id]->DrawTextA(0, buf, -1, &position, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_RGBA(p_color.r, p_color.g, p_color.b, p_color.a));
		}

		void draw_w(e_font font_id, int x, int y, color p_color, DWORD flag, const wchar_t* fmt, ...)
		{
			wchar_t buf[1024] = { '\0' };
			va_list va_alist;
			va_start(va_alist, fmt);
			vswprintf_s(buf, fmt, va_alist);
			va_end(va_alist);

			rect position = { x, y, 0, 0 };

			if (flag != null)
			{
				rect tsize = size_w(font_id, buf);
				if (flag & DT_CENTER) position.left -= tsize.left / 2;
				if (flag & DT_VCENTER)position.top -= tsize.top / 2;
				if (flag & DT_RIGHT)  position.left -= tsize.left;
				if (flag & DT_BOTTOM) position.top -= tsize.top;
			}

			fonts[font_id]->DrawTextW(0, buf, -1, &position, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_RGBA(p_color.r, p_color.g, p_color.b, p_color.a));
		}
	}

	namespace prim
	{
		/*struct vertex
		{
			/*vertex(float x, float y, dword color)
			{
				this->x = x;
				this->y = y;
				this->color = color;
			}//

			float x, y, z, rhw;
			dword color;
			float u, v;

			//float X, Y;// , Z, RHW;
			//DWORD Color;
			//float fU;
			//float fV;
		};*/

		/*struct XMFLOAT4
		{
			float x;
			float y;
			float z;
			float w;

			XMFLOAT4() = default;

			XMFLOAT4(const XMFLOAT4&) = default;
			XMFLOAT4& operator=(const XMFLOAT4&) = default;

			XMFLOAT4(XMFLOAT4&&) = default;
			XMFLOAT4& operator=(XMFLOAT4&&) = default;
			
			constexpr XMFLOAT4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
			explicit XMFLOAT4(_In_reads_(4) const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}
		};

		struct vertex_t
		{
			XMFLOAT4 xyzrhw;
			D3DCOLOR color;

			enum
			{
				FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE
			};
		};

		inline void build_vertex(XMFLOAT4 xyzrhw, D3DCOLOR color, vertex_t* vertex_list, int index)
		{
			vertex_list[index].xyzrhw = xyzrhw;
			//vertex_list[index].x = x;
			//vertex_list[index].y = y;
			//vertex_list[index].z = z;
			//vertex_list[index].rhw = rhw;
			vertex_list[index].color = color;
		}

		HRESULT generate_texture(IDirect3DTexture9** ppD3Dtex, DWORD colour32)
		{
			if (FAILED(device->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)))
				return E_FAIL;

			WORD colour16 = ((WORD)((colour32 >> 28) & 0xF) << 12)
				| (WORD)(((colour32 >> 20) & 0xF) << 8)
				| (WORD)(((colour32 >> 12) & 0xF) << 4)
				| (WORD)(((colour32 >> 4) & 0xF) << 0);
			D3DLOCKED_RECT d3dlr;
			(*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
			WORD* pDst16 = (WORD*)d3dlr.pBits;
			for (int xy = 0; xy < 8 * 8; xy++)
				*pDst16++ = colour16;
			(*ppD3Dtex)->UnlockRect(0);
			return S_OK;
		}

		IDirect3DTexture9* primitive = null;*/

		void filled_box(long x, long y, long x1, long y1, color p_color)
		{
			D3DRECT position = { x, y, x1, y1 };
			device->Clear(0x1, &position, D3DCLEAR_TARGET, p_color.to_d3d(), 0.f, 0x0);
			//if (generate_texture(&primitive, p_color.to_d3d()))
			//{
			//if (primitive == null) return;
			/*static IDirect3DVertexBuffer9* g_pVb = nullptr;

			if (!g_pVb)
				device->CreateVertexBuffer(4 * sizeof(vertex_t), 0, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &g_pVb, NULL);

			vertex_t vertexList[4];

			build_vertex(XMFLOAT4(x, y1, 0.f, 1.f), D3DCOLOR_ARGB(255, 255, 2, 200), vertexList, 0);
			build_vertex(XMFLOAT4(x, y, 0.f, 1.f), D3DCOLOR_ARGB(255, 255, 2, 200), vertexList, 1);
			build_vertex(XMFLOAT4(x1, y1, 0.f, 1.f), D3DCOLOR_ARGB(255, 255, 2, 200), vertexList, 2);
			build_vertex(XMFLOAT4(x1, y, 0.f, 1.f), D3DCOLOR_ARGB(255, 255, 2, 200), vertexList, 3);

			void* pVertices;

			g_pVb->Lock(0, 0, &pVertices, 0);
			memcpy(pVertices, vertexList, 4 * sizeof(vertex_t));
			g_pVb->Unlock();

			LPDIRECT3DSTATEBLOCK9 state_block = null;
			device->CreateStateBlock(D3DSBT_ALL, &state_block);
			state_block->Capture();

			device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
			device->SetTexture(0, primitive);
			device->SetStreamSource(0, g_pVb, 0, sizeof(vertex_t));

			device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			state_block->Apply();
			state_block->Release();*/

			//primitive->Release();
			//primitive = null;
		//}
		}

		void bordered_box(long x, long y, long x1, long y1, color p_color)
		{
			filled_box(x, y, x1, y + 1, p_color);  //  xy._____.x1y
			filled_box(x, y, x + 1, y1, p_color);  //    |     |
			filled_box(x, y1 - 1, x1, y1, p_color);//    |     |
			filled_box(x1 - 1, y, x1, y1, p_color);// xy1._____.x1y1
		}

		// не работает цвет (всегда черный)
		//void line(float x, float y, float x1, float y1, color p_color)
		//{
			/*vertex line_vertex[2];// = { { x, y, p_color.to_d3d()}, { x1, y1, p_color.to_d3d()} };
			line_vertex[0].x = x;
			line_vertex[0].y = y;
			line_vertex[0].color = p_color.to_d3d();
			line_vertex[1].x = x1;
			line_vertex[1].y = y1;
			line_vertex[1].color = p_color.to_d3d();
			device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
			device->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &line_vertex, sizeof(vertex));*/

			/*IDirect3DBaseTexture9* pTexture = NULL;
			DWORD dwLighting = 0, dwZEnable = 0, dwZFunc = 0, dwFVF = 0, dwColorOp = 0, dwColorArg = 0;

			vertex line_vertex[2];// = { { x, y, p_color.to_d3d()}, { x1, y1, p_color.to_d3d()} };
			line_vertex[0].x = x;
			line_vertex[0].y = y;
			line_vertex[0].color = D3DCOLOR_ARGB(p_color.a, p_color.r, p_color.g, p_color.b);//p_color.to_d3d();
			line_vertex[1].x = x1;
			line_vertex[1].y = y1;
			line_vertex[1].color = D3DCOLOR_ARGB(p_color.a, p_color.r, p_color.g, p_color.b);//p_color.to_d3d();

			device->GetTexture(0, &pTexture);
			device->GetRenderState(D3DRS_LIGHTING, &dwLighting);
			device->GetRenderState(D3DRS_ZENABLE, &dwZEnable);
			device->GetRenderState(D3DRS_ZFUNC, &dwZFunc);
			device->GetFVF(&dwFVF);
			device->GetTextureStageState(0, D3DTSS_COLOROP, &dwColorOp);
			device->GetTextureStageState(0, D3DTSS_COLORARG1, &dwColorArg);

			device->SetTexture(0, NULL);
			device->SetRenderState(D3DRS_LIGHTING, FALSE);
			device->SetRenderState(D3DRS_ZENABLE, FALSE);
			device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
			device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

			device->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &line_vertex, sizeof(vertex));

			device->SetTexture(0, pTexture);
			device->SetRenderState(D3DRS_LIGHTING, dwLighting);
			device->SetRenderState(D3DRS_ZENABLE, dwZEnable);
			device->SetRenderState(D3DRS_ZFUNC, dwZFunc);
			device->SetFVF(dwFVF);
			device->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp);
			device->SetTextureStageState(0, D3DTSS_COLORARG1, dwColorArg);
		}*/
	}
	
	rect get_screen_size()
	{
		D3DVIEWPORT9 viewport;
		device->GetViewport(&viewport);
		return { (int)viewport.X, (int)viewport.Y, (int)viewport.Width, (int)viewport.Height };
	}

	//(c) tgf a5
	bool screen_transform(const Vector& point, Vector& screen)
	{
		float w;
		auto worldToScreen = _engine->w2s_matrix();

		screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
		screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
		w = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
		screen.z = 0.0f;

		bool behind = false;

		if (w < 0.001f)
		{
			behind = true;
			screen.x *= 100000;
			screen.y *= 100000;
		}
		else
		{
			behind = false;
			float invw = 1.0f / w;
			screen.x *= invw;
			screen.y *= invw;
		}

		return behind;
	}

	//(c) tgf a5
	bool w2s(const cvector& point, cvector& screen)
	{
		if (!screen_transform(point, screen))
		{
			float x = global::screen.right / 2;
			float y = global::screen.bottom / 2;
			x += 0.5 * screen.x * global::screen.right + 0.5;
			y -= 0.5 * screen.y * global::screen.bottom + 0.5;
			screen.x = x;
			screen.y = y;
			return true;
		}

		return false;
	}
}