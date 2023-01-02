#pragma once
#include "includes.h"
#include "definitions.h"
//#include "console.h"

#define INRANGE(x,a,b)    (x >= a && x <= b)
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

namespace memory
{
	// функция получения оффсета
	inline dword pattern(std::string moduleName, std::string pattern)
	{
		const char* pat = pattern.c_str();
		DWORD firstMatch = 0;
		DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
		MODULEINFO miModInfo;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
		for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
		{
			if (!*pat)
				return firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
			{
				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else
					pat += 2; //one ?
			}
			else
			{
				pat = pattern.c_str();
				firstMatch = 0;
			}
		}
		return NULL;
	}

	// функция получения интерфейса
	template< typename T >
	T* pinterface(std::string strModule, std::string strInterface)
	{
		typedef T* (*CreateInterfaceFn)(const char* szName, int iReturn);
		CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(strModule.c_str()), "CreateInterface");

		std::cout << ("/iface/ " + strModule + " : ") << std::hex << (dword)CreateInterface << endl;;

		return CreateInterface(strInterface.c_str(), 0);
	}

	// класс для выполнения хуков
	class vthook
	{
	public:
		vthook()
		{
			memset(this, 0, sizeof(vthook));
		}

		vthook(PDWORD* ppdwClassBase)
		{
			initialize(ppdwClassBase);
		}

		~vthook()
		{
			unhook();
		}

		bool initialize(PDWORD* pp_class_base)
		{
			class_base = pp_class_base;
			oldvt = *pp_class_base;
			vtsize = get_vt_count(*pp_class_base);
			newvt = new DWORD[vtsize];
			memcpy(newvt, oldvt, sizeof(DWORD) * vtsize);
			*pp_class_base = newvt;
			return true;
		}

		bool initialize(PDWORD** pp_class_base) // fix for pp
		{
			return initialize(*pp_class_base);
		}

		void unhook()
		{
			if (class_base)
			{
				*class_base = oldvt;
			}
		}

		void rehook()
		{
			if (class_base)
			{
				*class_base = newvt;
			}
		}

		int get_func_count()
		{
			return (int)vtsize;
		}

		DWORD get_func_address(int Index)
		{
			if (Index >= 0 && Index <= (int)vtsize && oldvt != NULL)
			{
				return oldvt[Index];
			}
			return NULL;
		}

		PDWORD get_oldvt()
		{
			return oldvt;
		}

		DWORD hook_function(DWORD dwNewFunc, unsigned int iIndex)
		{
			if (newvt && oldvt && iIndex <= vtsize && iIndex >= 0)
			{
				newvt[iIndex] = dwNewFunc;
				return oldvt[iIndex];
			}

			return NULL;
		}

	private:
		DWORD get_vt_count(PDWORD vmt)
		{
			DWORD dwIndex = 0;

			for (dwIndex = 0; vmt[dwIndex]; dwIndex++)
			{
				if (IsBadCodePtr((FARPROC)vmt[dwIndex]))
				{
					break;
				}
			}
			return dwIndex;
		}

		PDWORD* class_base;
		PDWORD newvt, oldvt;
		DWORD vtsize;
	};
}

template< typename T >
T vfunc(void* vTable, int iIndex)
{
	return (*(T**)vTable)[iIndex];
}

template <typename Fn> __forceinline Fn CallVirtualFunction(void* pClassBase, int nFunctionIndex) // (c) гузей
{
	return (Fn)((PDWORD) * (PDWORD*)pClassBase)[nFunctionIndex];
}