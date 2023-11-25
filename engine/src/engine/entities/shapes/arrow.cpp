#include "pch.h"
#include "arrow.h"
#include "engine.h"

engine::arrow::arrow(float size) : m_depth(.1f), m_size(size)
{
	std::vector<engine::mesh::vertex> arrow_vertices =
	{
		// front face
		{{0.f,	0.5f * m_size,	m_depth},	glm::vec3(0.f,0.f,-1.f),			{0.5f,1.f}},
		{{1.f * m_size,	0.f,	m_depth},	glm::vec3(0.f,0.f,-1.f),			{1.f,0.5f}},
		{{1.f * m_size,	-0.5f * m_size,	m_depth},	glm::vec3(0.f,0.f,-1.f),	{1.f,0.f}},
		{{0.f,	0.f,	m_depth},	glm::vec3(0.f,0.f,-1.f),					{0.f,0.5f}},
		{{-1.f * m_size,	-0.5f * m_size,	m_depth},	glm::vec3(0.f,0.f,-1.f),	{-1.f,0.f}},
		{{-1.f * m_size,	0.f,	m_depth},	glm::vec3(0.f,0.f,-1.f),			{-1.f,0.5f}},
		// back face
		{{0.f,	0.5f * m_size,	-m_depth},	glm::vec3(0.f,0.f,1.f),				{0.5f,1.f}},
		{{1.f * m_size,	0.f,	-m_depth},	glm::vec3(0.f,0.f,1.f),				{1.f,0.5f}},
		{{1.f * m_size,	-0.5f * m_size,	-m_depth},	glm::vec3(0.f,0.f,1.f),		{1.f,0.f}},
		{{0.f,	0.f,	-m_depth},			glm::vec3(0.f,0.f,1.f),				{0.f,0.5f}},
		{{-1.f * m_size,	-0.5f * m_size,	-m_depth},	glm::vec3(0.f,0.f,1.f),		{-1.f,0.f}},
		{{-1.f * m_size,	0.f,	-m_depth},	glm::vec3(0.f,0.f,1.f),				{-1.f,0.5f}},
	};

	std::vector<glm::vec3> normals;
	// We need 6 surfaces between two faces
	normals.push_back(glm::cross(arrow_vertices[7].position - arrow_vertices[1].position, arrow_vertices[6].position - arrow_vertices[1].position));
	normals.push_back(glm::cross(arrow_vertices[8].position - arrow_vertices[2].position, arrow_vertices[7].position - arrow_vertices[2].position));
	normals.push_back(glm::cross(arrow_vertices[9].position - arrow_vertices[3].position, arrow_vertices[8].position - arrow_vertices[3].position));
	normals.push_back(glm::cross(arrow_vertices[10].position - arrow_vertices[4].position, arrow_vertices[9].position - arrow_vertices[4].position));
	normals.push_back(glm::cross(arrow_vertices[11].position - arrow_vertices[5].position, arrow_vertices[10].position - arrow_vertices[5].position));
	normals.push_back(glm::cross(arrow_vertices[6].position - arrow_vertices[5].position, arrow_vertices[11].position - arrow_vertices[5].position));

	std::vector<mesh::vertex> between_vertices
	{
		{arrow_vertices[1].position,	normals[0],	{0.f,0.f}},
		{arrow_vertices[6].position,	normals[0],	{1.f,0.f}},
		{arrow_vertices[0].position,	normals[0],	{.5f,1.f}},

		{arrow_vertices[1].position,	normals[0],	{0.f,0.f}},
		{arrow_vertices[7].position,	normals[0],	{1.f,0.f}},
		{arrow_vertices[6].position,	normals[0],	{.5f,1.f}},

		{arrow_vertices[2].position,	normals[1],	{0.f,0.f}},
		{arrow_vertices[7].position,	normals[1],	{1.f,0.f}},
		{arrow_vertices[1].position,	normals[1],	{.5f,1.f}},

		{arrow_vertices[2].position,	normals[1],	{0.f,0.f}},
		{arrow_vertices[8].position,	normals[1],	{1.f,0.f}},
		{arrow_vertices[7].position,	normals[1],	{.5f,1.f}},

		{arrow_vertices[3].position,	normals[2],	{0.f,0.f}},
		{arrow_vertices[8].position,	normals[2],	{1.f,0.f}},
		{arrow_vertices[2].position,	normals[2],	{.5f,1.f}},

		{arrow_vertices[3].position,	normals[2],	{0.f,0.f}},
		{arrow_vertices[9].position,	normals[2],	{1.f,0.f}},
		{arrow_vertices[8].position,	normals[2],	{.5f,1.f}},

		{arrow_vertices[4].position,	normals[3],	{0.f,0.f}},
		{arrow_vertices[9].position,	normals[3],	{1.f,0.f}},
		{arrow_vertices[3].position,	normals[3],	{.5f,1.f}},

		{arrow_vertices[4].position,	normals[3],	{0.f,0.f}},
		{arrow_vertices[10].position,	normals[3],	{1.f,0.f}},
		{arrow_vertices[9].position,	normals[3],	{.5f,1.f}},

		{arrow_vertices[5].position,	normals[4],	{0.f,0.f}},
		{arrow_vertices[10].position,	normals[4],	{1.f,0.f}},
		{arrow_vertices[4].position,	normals[4],	{.5f,1.f}},

		{arrow_vertices[5].position,	normals[4],	{0.f,0.f}},
		{arrow_vertices[11].position,	normals[4],	{1.f,0.f}},
		{arrow_vertices[10].position,	normals[4],	{.5f,1.f}},

		{arrow_vertices[6].position,	normals[5],	{0.f,0.f}},
		{arrow_vertices[11].position,	normals[5],	{1.f,0.f}},
		{arrow_vertices[5].position,	normals[5],	{.5f,1.f}},

		{arrow_vertices[0].position,	normals[5],	{0.f,0.f}},
		{arrow_vertices[6].position,	normals[5],	{1.f,0.f}},
		{arrow_vertices[5].position,	normals[5],	{.5f,1.f}},
	};
	// add surface in between to the faces of the arrow
	arrow_vertices.insert(arrow_vertices.end(), between_vertices.begin(), between_vertices.end());

	std::vector<uint32_t> arrow_indices =
	{
		1,0,3,
		2,1,3,
		3,5,4,
		3,0,5,

		6,7,9,
		9,7,8,
		11,9,10,
		11,6,9,

		12,13,14,
		15,16,17,
		18,19,20,
		21,22,23,
		24,25,26,
		27,28,29,
		30,31,32,
		33,34,35,
		36,37,38,
		39,40,41,
		42,43,44,
		45,46,47
	};
	m_arrow_mesh = engine::mesh::create(arrow_vertices, arrow_indices);
}

engine::arrow::~arrow()
{
}

engine::ref<engine::arrow> engine::arrow::create(float size)
{
	return std::make_shared<engine::arrow>(size);
}
