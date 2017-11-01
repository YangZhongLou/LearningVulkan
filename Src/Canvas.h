#pragma once

#include <string>
#ifdef _WIN32
#include <Windows.h>
#endif
#include "Renderer.h"
#include "WindowParameters.h"

namespace yzl
{
	class Renderer;

	class Canvas
	{
	public:
		Canvas(std::string title, int x, int y, int width, int height);
		~Canvas();

	public:
		void Draw();

		const HINSTANCE& GetHInstance() const { return m_winParams.hinstance; }
		const HWND& GetHWnd() const { return m_winParams.hwnd; }

	private:
		void InitWindow();
	private:
		std::string m_title;
		int m_x;
		int m_y;
		int m_width;
		int m_height;

		Renderer* m_renderer;
		WindowParameters m_winParams;
	};
}