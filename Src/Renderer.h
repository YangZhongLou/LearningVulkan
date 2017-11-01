#pragma once

namespace yzl
{
	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

	public:
		virtual void Submit();
		virtual void Flush();
	};
}