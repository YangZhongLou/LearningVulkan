#pragma once

#include <string>
#include "Define.h"

namespace yzl
{
	class Canvas;
	class Renderer;
	class VulkanBuffer;

	class Application
	{
	public:
		Application();
		Application(std::string name, int x, int y, int width, int height);
		virtual ~Application();
	public:
		void Run();

		Renderer * GetRenderer() const { return m_renderer; }
		void Init(std::string name, int x = 100, int y = 100, int width = 100, int height = 100);
	private:
		Renderer * m_renderer;
		Canvas * m_canvas;
	};
}