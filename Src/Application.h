#pragma once

#include <string>
#include "Define.h"

namespace yzl
{
	class Canvas;

	class Application
	{
	public:
		Application();
		Application(std::string name, int x, int y, int width, int height);
		~Application();
	public:
		void Run();
	private:
		void Init(std::string name, int x = 100, int y = 100, int width = 100, int height = 100);
	private:
		Canvas* m_canvas;
	};
}