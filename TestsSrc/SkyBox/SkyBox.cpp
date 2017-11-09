#include <Mesh.h>
#include "SkyBox.h"
#include <Utils.h>


namespace yzl
{
	SkyBox::SkyBox(std::string name, int x, int y, int width, int height)
	{
		Init(name, x, y, width, height);
	}

	SkyBox::~SkyBox()
	{
		SAFE_DELETE(m_mesh);
	}

	Mesh * SkyBox::LoadMesh(char const * filename,
		bool loadNormals, 
		bool loadTexcoords, 
		bool generateTangentSpaceVectors, 
		bool unify, 
		uint32_t * vertexStride)
	{
		m_mesh = new Mesh(filename, loadNormals, loadTexcoords, generateTangentSpaceVectors, unify, vertexStride);
		return m_mesh;
	}


}
