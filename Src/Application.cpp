#include "Application.h"
#include "Canvas.h"
#include "Utils.h"
#include "Vulkan/VulkanFunctions.h"

namespace yzl
{
	Application::Application()
	{
		Init("default");
	}

	Application::Application(std::string name, int x, int y, int width, int height)
	{
		Init(name, x, y, width, height);
	}

	void Application::Init(std::string name, int x, int y, int width, int height)
	{
		m_canvas = new Canvas(name, x, y, width, height);
	}

	Application::~Application()
	{
		SAFE_DELETE(m_canvas);
	}

	void Application::Run()
	{
		MSG message;
		bool loop = true;

		while (loop) 
		{
			if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) 
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else
			{
				m_canvas->Draw();
			}
		}
	}
}