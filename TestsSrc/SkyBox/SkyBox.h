#pragma once

#include <Application.h>

namespace yzl
{
	class Mesh;

	class SkyBox : public Application
	{
	public:
		SkyBox(std::string name, int x, int y, int width, int height);
		~SkyBox();
	public:
		Mesh * LoadMesh(char const * filename,
			bool loadNormals,
			bool loadTexcoords,
			bool generateTangentSpaceVectors,
			bool unify,
			uint32_t * vertexStride = nullptr);
		Mesh * GetMesh() { return m_mesh; }
	private:
		Mesh * m_mesh;
	};
}