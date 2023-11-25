#include "pch.h"
#include "engine.h"
#include "spike.h"

engine::spike::spike(float size) : m_size(size)
{
	std::vector<glm::vec3> positions =
	{
		// Base square
		{-.5f,0.f ,.5f},
		{.5f,0.f ,.5f},
		{.5f,0.f ,-.5f},
		{-.5f,0.f ,-.5f},
		// Smaller square
		{-.25f,1.6f,.25f},
		{.25f,1.6f,.25f},
		{.25f,1.6f,-.25f},
		{-.25f,1.6f,-.25f},
		// Peak
		{0.f,2.f,0.f},
	};

	std::vector<glm::vec3> normals;
	// Lower level normals
	// front
	normals.push_back(glm::cross((positions[5] - positions[1]), (positions[4] - positions[1])));
	// right
	normals.push_back(glm::cross((positions[6] - positions[2]), (positions[5] - positions[2])));
	// back
	normals.push_back(glm::cross((positions[7] - positions[3]), (positions[6] - positions[3])));
	// left
	normals.push_back(glm::cross((positions[4] - positions[0]), (positions[4] - positions[3])));

	// peak (Upper)
	// front
	normals.push_back(glm::cross((positions[4] - positions[8]), (positions[5] - positions[8])));
	// right
	normals.push_back(glm::cross((positions[5] - positions[8]), (positions[6] - positions[8])));
	// back
	normals.push_back(glm::cross((positions[6] - positions[8]), (positions[7] - positions[8])));
	// left
	normals.push_back(glm::cross((positions[7] - positions[8]), (positions[4] - positions[8])));

	std::vector<engine::mesh::vertex> spike_vertices =
	{
		// Base
		{positions[0],{0.f,-1.f,0.f},{0.f,0.f}},
		{positions[1],{0.f,-1.f,0.f},{0.f,1.f}},
		{positions[2],{0.f,-1.f,0.f},{1.f,1.f}},
		{positions[3],{0.f,-1.f,0.f},{1.f,0.f}},
		// Lower face front
		{positions[4],normals[0],{1.f,1.f}}, // 4
		{positions[5],normals[0],{0.f,1.f}}, // 5
		{positions[0],normals[0],{0.f,0.f}}, // 6
		{positions[1],normals[0],{1.f,0.f}}, // 7
		// Lower face right
		{positions[5],normals[1],{1.f,1.f}}, // 8
		{positions[6],normals[1],{0.f,1.f}}, // 9
		{positions[1],normals[1],{0.f,0.f}}, // 10
		{positions[2],normals[1],{1.f,0.f}}, // 11
		// Lower face back
		{positions[6],normals[2],{1.f,1.f}}, // 12
		{positions[7],normals[2],{0.f,1.f}}, // 13
		{positions[2],normals[2],{0.f,0.f}}, // 14
		{positions[3],normals[2],{1.f,0.f}}, // 15
		// Lower face left
		{positions[7],normals[3],{1.f,1.f}}, // 16
		{positions[4],normals[3],{0.f,1.f}}, // 17
		{positions[3],normals[3],{0.f,0.f}}, // 18
		{positions[0],normals[3],{1.f,0.f}}, // 19

		// Upper face front
		{positions[8],normals[4],{ 0.f, 0.f }},
		{positions[4],normals[4],{ 1.f, 0.f }},
		{positions[5],normals[4],{ 0.5f, 1.f }},
		// Upper face right
		{positions[8],normals[5],{ 0.f, 0.f }},
		{positions[5],normals[5],{ 1.f, 0.f }},
		{positions[6],normals[5],{ 0.5f, 1.f }},
		// Upper face back
		{positions[8],normals[6],{ 0.f, 0.f }},
		{positions[6],normals[6],{ 1.f, 0.f }},
		{positions[7],normals[6],{ 0.5f, 1.f }},
		// Upper face left
		{positions[8],normals[7],{ 0.f, 0.f }},
		{positions[7],normals[7],{ 1.f, 0.f }},
		{positions[4],normals[7],{ 0.5f, 1.f }},

	};

	const std::vector<uint32_t> spike_indices
	{
		0,2,1, 0,3,2, // Base
		// Lower
		6,7,5, 6,5,4,
		10,9,8, 10,11,9,
		15,13,12, 14,15,12,
		19,17,16, 19,16,18,
		// Peak (Upper)
		20,21,22,
		23,24,25,
		26,27,28,
		29,30,31
	};

	m_spike_mesh = engine::mesh::create(spike_vertices, spike_indices);
}

engine::spike::~spike()
{
}

engine::ref<engine::spike> engine::spike::create(float size)
{
	return std::make_shared<engine::spike>(size);
}
