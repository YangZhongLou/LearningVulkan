
#include "Canvas.h"
#include "VkRenderer.h"

namespace yzl
{
	Canvas::Canvas(std::string title, int x, int y, int width, int height)
	{
		m_title = title;
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;

		InitWindow();

		m_renderer = new VkRenderer();
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		//struct sample_info *info = reinterpret_cast<struct sample_info *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		switch (uMsg) {
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			//run(info);
			return 0;
		default:
			break;
		}
		return (DefWindowProc(hWnd, uMsg, wParam, lParam));
	}

	void Canvas::InitWindow()
	{
		WNDCLASSEX win_class;

		win_class.cbSize = sizeof(WNDCLASSEX);
		win_class.style = CS_HREDRAW | CS_VREDRAW;
		win_class.lpfnWndProc = WndProc;
		win_class.cbClsExtra = 0;
		win_class.cbWndExtra = 0;
		win_class.hInstance = m_winParams.hinstance;  // hInstance
		win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
		win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		win_class.lpszMenuName = NULL;
		win_class.lpszClassName = m_title.c_str();
		win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
		if (!RegisterClassEx(&win_class)) {
			printf("Unexpected error trying to start the application!\n");
			fflush(stdout);
			exit(1);
		}

		RECT wr = { 0, 0, m_width, m_height };
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
		m_winParams.hwnd = CreateWindowEx(0,
			m_title.c_str(),            
			m_title.c_str(),             
			WS_OVERLAPPEDWINDOW |  
			WS_VISIBLE | WS_SYSMENU,
			m_x, m_y,
			wr.right - wr.left,  
			wr.bottom - wr.top,  
			NULL,              
			NULL,                
			m_winParams.hinstance,
			NULL);              
		if (!m_winParams.hwnd) {
			printf("Cannot create a window in which to draw!\n");
			fflush(stdout);
			exit(1);
		}
		//SetWindowLongPtr(info.window, GWLP_USERDATA, (LONG_PTR)&info);
	}

	Canvas::~Canvas()
	{
	}

	void Canvas::Draw()
	{
		m_renderer->Flush();
	}
}