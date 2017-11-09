#pragma once

#include <vector>
#include "Math/Math.h"

namespace yzl
{
	struct Part 
	{
		uint32_t  vertexOffset;
		uint32_t  vertexCount;
	};

	class Mesh
	{
	public:
		Mesh(char const * filename,
			bool loadNormals,
			bool loadTexcoords,
			bool generateTangentSpaceVectors,
			bool unify,
			uint32_t * vertexStride = nullptr);
		~Mesh();
	public:
		const std::vector<float> & GetData() { return m_data; }
	private:
		bool Init(char const * filename,
			bool loadNormals,
			bool loadTexcoords,
			bool generateTangentSpaceVectors,
			bool unify,
			uint32_t * vertexStride = nullptr);

		void GenerateTangentSpaceVectors();
		void CalculateTangentAndBitangent(
			float const * normalData,
			Vector3 const & faceTangent,
			Vector3 const & faceBitangent,
			float * tangentData,
			float * bitangentData);

	private:
		std::vector<float>  m_data;
		std::vector<Part>   m_parts;
	};
}