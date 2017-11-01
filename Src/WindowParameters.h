#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

namespace yzl
{
	typedef struct WindowParameters
	{
#ifdef _WIN32
		HINSTANCE          hinstance;
		HWND               hwnd;
#endif
	}WindowParameters;
}