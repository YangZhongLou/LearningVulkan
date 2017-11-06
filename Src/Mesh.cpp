#define TINYOBJLOADER_IMPLEMENTATION
#include <mesh/tiny_obj_loader.h>
#include "Mesh.h"
#include "Math/Math.h"
#include <iostream>

namespace yzl
{
	Mesh::Mesh(char const * filename,
		bool loadNormals,
		bool loadTexcoords,
		bool generateTangentSpaceVectors,
		bool unify,
		uint32_t * vertexStride /* = nullptr */)
	{
		Init(filename, loadNormals, loadTexcoords, generateTangentSpaceVectors, unify, vertexStride);
	}

	Mesh::~Mesh()
	{

	}

	bool Mesh::Init(char const * filename, 
		bool loadNormals, 
		bool loadTexcoords, 
		bool generateTangentSpaceVectors, 
		bool unify, 
		uint32_t * vertexStride)
	{
		tinyobj::attrib_t attribs;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string error;

		bool result = tinyobj::LoadObj(&attribs, &shapes, &materials, &error, filename);
		if (!result)
		{
			std::cout << "Could not open the '" << filename << "' file.";
			if (0 < error.size())
			{
				std::cout << " " << error;
			}
			std::cout << std::endl;
			return false;
		}

		// Normal vectors and texture coordinates are required to generate tangent and bitangent vectors
		if (!loadNormals || !loadTexcoords) 
		{
			generateTangentSpaceVectors = false;
		}

		// Load model data and unify (normalize) its size and position
		float minX = attribs.vertices[0];
		float maxX = attribs.vertices[0];
		float minY = attribs.vertices[1];
		float maxY = attribs.vertices[1];
		float minZ = attribs.vertices[2];
		float maxZ = attribs.vertices[2];

		uint32_t offset = 0;
		for (auto & shape : shapes) 
		{
			uint32_t part_offset = offset;

			for (auto & index : shape.mesh.indices) 
			{
				m_data.emplace_back(attribs.vertices[3 * index.vertex_index + 0]);
				m_data.emplace_back(attribs.vertices[3 * index.vertex_index + 1]);
				m_data.emplace_back(attribs.vertices[3 * index.vertex_index + 2]);
				++offset;

				if (loadNormals) 
				{
					if (attribs.normals.size() == 0) 
					{
						std::cout << "Could not load normal vectors data in the '" << filename << "' file.";
						return false;
					}
					else 
					{
						m_data.emplace_back(attribs.normals[3 * index.normal_index + 0]);
						m_data.emplace_back(attribs.normals[3 * index.normal_index + 1]);
						m_data.emplace_back(attribs.normals[3 * index.normal_index + 2]);
					}
				}

				if (loadTexcoords) 
				{
					if (attribs.texcoords.size() == 0) 
					{
						std::cout << "Could not load texture coordinates data in the '" << filename << "' file.";
						return false;
					}
					else 
					{
						m_data.emplace_back(attribs.texcoords[2 * index.texcoord_index + 0]);
						m_data.emplace_back(attribs.texcoords[2 * index.texcoord_index + 1]);
					}
				}

				if (generateTangentSpaceVectors) 
				{
					// Insert temporary tangent space vectors data
					for (int i = 0; i < 6; ++i) 
					{
						m_data.emplace_back(0.0f);
					}
				}

				if (unify) 
				{
					if (attribs.vertices[3 * index.vertex_index + 0] < minX) 
					{
						minX = attribs.vertices[3 * index.vertex_index + 0];
					}
					if (attribs.vertices[3 * index.vertex_index + 0] > maxX) 
					{
						maxX = attribs.vertices[3 * index.vertex_index + 0];
					}
					if (attribs.vertices[3 * index.vertex_index + 1] < minY) 
					{
						minY = attribs.vertices[3 * index.vertex_index + 1];
					}
					if (attribs.vertices[3 * index.vertex_index + 1] > maxY) 
					{
						maxY = attribs.vertices[3 * index.vertex_index + 1];
					}
					if (attribs.vertices[3 * index.vertex_index + 2] < minZ) 
					{
						minZ = attribs.vertices[3 * index.vertex_index + 2];
					}
					if (attribs.vertices[3 * index.vertex_index + 2] > maxZ) 
					{
						maxZ = attribs.vertices[3 * index.vertex_index + 2];
					}
				}
			}

			uint32_t part_vertex_count = offset - part_offset;
			if (0 < part_vertex_count) 
			{
				m_parts.push_back({ part_offset, part_vertex_count });
			}
		}

		uint32_t stride = 3 + (loadNormals ? 3 : 0) + (loadTexcoords ? 2 : 0) + (generateTangentSpaceVectors ? 6 : 0);
		if (vertexStride) 
		{
			*vertexStride = stride * sizeof(float);
		}

		if (generateTangentSpaceVectors) 
		{
			GenerateTangentSpaceVectors();
		}

		if (unify) 
		{
			float offset_x = 0.5f * (minX + maxX);
			float offset_y = 0.5f * (minY + maxY);
			float offset_z = 0.5f * (minZ + maxZ);
			float scale_x = abs(minX - offset_x) > abs(maxX - offset_x) ? abs(minX - offset_x) : abs(maxX - offset_x);
			float scale_y = abs(minY - offset_y) > abs(maxY - offset_y) ? abs(minY - offset_y) : abs(maxY - offset_y);
			float scale_z = abs(minZ - offset_z) > abs(maxZ - offset_z) ? abs(minZ - offset_z) : abs(maxZ - offset_z);
			float scale = scale_x > scale_y ? scale_x : scale_y;
			scale = scale_z > scale ? 1.0f / scale_z : 1.0f / scale;

			for (size_t i = 0; i < m_data.size() - 2; i += stride) 
			{
				m_data[i + 0] = scale * (m_data[i + 0] - offset_x);
				m_data[i + 1] = scale * (m_data[i + 1] - offset_y);
				m_data[i + 2] = scale * (m_data[i + 2] - offset_z);
			}
		}

		return true;
	}

	void Mesh::GenerateTangentSpaceVectors()
	{
		size_t const normal_offset = 3;
		size_t const texcoord_offset = 6;
		size_t const tangent_offset = 8;
		size_t const bitangent_offset = 11;
		size_t const stride = bitangent_offset + 3;

		for (auto & part : m_parts) 
		{
			for (size_t i = 0; i < m_data.size(); i += stride * 3) {
				size_t i1 = i;
				size_t i2 = i1 + stride;
				size_t i3 = i2 + stride;
				Vector3 const v1 = { m_data[i1], m_data[i1 + 1], m_data[i1 + 2] };
				Vector3 const v2 = { m_data[i2], m_data[i2 + 1], m_data[i2 + 2] };
				Vector3 const v3 = { m_data[i3], m_data[i3 + 1], m_data[i3 + 2] };

				std::array<float, 2> const w1 = { m_data[i1 + texcoord_offset], m_data[i1 + texcoord_offset + 1] };
				std::array<float, 2> const w2 = { m_data[i2 + texcoord_offset], m_data[i2 + texcoord_offset + 1] };
				std::array<float, 2> const w3 = { m_data[i3 + texcoord_offset], m_data[i3 + texcoord_offset + 1] };

				float x1 = v2[0] - v1[0];
				float x2 = v3[0] - v1[0];
				float y1 = v2[1] - v1[1];
				float y2 = v3[1] - v1[1];
				float z1 = v2[2] - v1[2];
				float z2 = v3[2] - v1[2];

				float s1 = w2[0] - w1[0];
				float s2 = w3[0] - w1[0];
				float t1 = w2[1] - w1[1];
				float t2 = w3[1] - w1[1];

				float r = 1.0f / (s1 * t2 - s2 * t1);
				Vector3 faceTangent = { (t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r };
				Vector3 faceBitangent = { (s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r };

				CalculateTangentAndBitangent(&m_data[i1 + normal_offset], faceTangent, faceBitangent, &m_data[i1 + tangent_offset], &m_data[i1 + bitangent_offset]);
				CalculateTangentAndBitangent(&m_data[i2 + normal_offset], faceTangent, faceBitangent, &m_data[i2 + tangent_offset], &m_data[i2 + bitangent_offset]);
				CalculateTangentAndBitangent(&m_data[i3 + normal_offset], faceTangent, faceBitangent, &m_data[i3 + tangent_offset], &m_data[i3 + bitangent_offset]);
			}
		}
	}

	void Mesh::CalculateTangentAndBitangent(float const * normalData, 
		Vector3 const & faceTangent, 
		Vector3 const & faceBitangent, 
		float * tangentData, 
		float * bitangentData)
	{
		// Gram-Schmidt orthogonalize
		Vector3 const normal = { normalData[0], normalData[1], normalData[2] };
		Vector3 const tangent = Normalize(faceTangent - normal * Dot(normal, faceTangent));

		// Calculate handedness
		float handedness = (Dot(Cross(normal, tangent), faceBitangent) < 0.0f) ? -1.0f : 1.0f;

		Vector3 const bitangent = handedness * Cross(normal, tangent);

		tangentData[0] = tangent[0];
		tangentData[1] = tangent[1];
		tangentData[2] = tangent[2];

		bitangentData[0] = bitangent[0];
		bitangentData[1] = bitangent[1];
		bitangentData[2] = bitangent[2];
	}
}