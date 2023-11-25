#include "pch.h" 
#include "heart.h" 
#include <engine.h> 

engine::heart::heart(float size) : m_size(size)
{
	std::vector<glm::vec3> normals;
	std::vector<engine::mesh::vertex> heart_vertices;
	const float depth = 0.2f;
	const float radius = m_size;

	// Face of the heart
	float y_position = tan(engine::PI / 3) * 2 * radius - radius / 2;
	for (int i = 0; i < 6; i++)
	{
		float theta = i * (360 / 6) * (engine::PI / 180);
		engine::mesh::vertex vertex(
			glm::vec3(radius * cos(theta) + radius, radius * sin(theta) + y_position, depth),
			glm::vec3(0.f, 0.f, -1.f),
			glm::vec2(cos(theta) / 2.0f + 0.25f, sin(theta) / 2.0f + 0.25f)
		);

		heart_vertices.push_back(vertex);
	}
	// Face front left
	for (int i = 0; i < 6; i++)
	{
		float theta = i * (360 / 6) * (engine::PI / 180);
		engine::mesh::vertex vertex(
			glm::vec3(radius * cos(theta) - radius, radius * sin(theta) + y_position, depth),
			glm::vec3(0.f, 0.f, -1.f),
			glm::vec2(cos(theta) / 2.0f + 0.25f, sin(theta) / 2.0f + 0.25f)
		);

		heart_vertices.push_back(vertex);
	}
	engine::mesh::vertex vertex(
		glm::vec3(0.f, 0.f, depth),
		glm::vec3(0.f, 0.f, -1.f),
		glm::vec2(0.f, 0.f)
	);
	heart_vertices.push_back(vertex);

	// Back face right side of the heart
	for (int i = 0; i < 6; i++)
	{
		float theta = i * (360 / 6) * (engine::PI / 180);
		engine::mesh::vertex vertex(
			glm::vec3(radius * cos(theta) + radius, radius * sin(theta) + y_position, -depth),
			glm::vec3(0.f, 0.f, 1.f),
			glm::vec2(cos(theta) / 2.0f + 0.25f, sin(theta) / 2.0f + 0.25f)
		);

		heart_vertices.push_back(vertex);
	}
	// Back face left side of the heart
	for (int i = 0; i < 6; i++)
	{
		float theta = i * (360 / 6) * (engine::PI / 180);
		engine::mesh::vertex vertex(
			glm::vec3(radius * cos(theta) - radius, radius * sin(theta) + y_position, -depth),
			glm::vec3(0.f, 0.f, 1.f),
			glm::vec2(cos(theta) / 2.0f + 0.25f, sin(theta) / 2.0f + 0.25f)
		);

		heart_vertices.push_back(vertex);
	}
	engine::mesh::vertex vertex_1(
		glm::vec3(0.f, 0.f, -depth),
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec2(0.f, 0.f)
	);
	heart_vertices.push_back(vertex_1);

	// We need 10 surfaces between two heart so we define 10 normals
	normals.push_back(glm::cross(heart_vertices[0].position - heart_vertices[13].position, heart_vertices[0].position - heart_vertices[14].position));
	normals.push_back(glm::cross(heart_vertices[1].position - heart_vertices[14].position, heart_vertices[1].position - heart_vertices[15].position));
	normals.push_back(glm::cross(heart_vertices[2].position - heart_vertices[15].position, heart_vertices[2].position - heart_vertices[16].position));
	normals.push_back(glm::cross(heart_vertices[5].position - heart_vertices[18].position, heart_vertices[5].position - heart_vertices[13].position));

	normals.push_back(glm::cross(heart_vertices[6].position - heart_vertices[19].position, heart_vertices[6].position - heart_vertices[20].position));
	normals.push_back(glm::cross(heart_vertices[7].position - heart_vertices[20].position, heart_vertices[7].position - heart_vertices[21].position));
	normals.push_back(glm::cross(heart_vertices[8].position - heart_vertices[21].position, heart_vertices[8].position - heart_vertices[22].position));
	normals.push_back(glm::cross(heart_vertices[9].position - heart_vertices[22].position, heart_vertices[9].position - heart_vertices[23].position));


	normals.push_back(glm::cross(heart_vertices[12].position - heart_vertices[25].position, heart_vertices[12].position - heart_vertices[18].position));
	normals.push_back(glm::cross(heart_vertices[12].position - heart_vertices[25].position, heart_vertices[12].position - heart_vertices[23].position));

	// Sides in between
	std::vector<engine::mesh::vertex> heart_side_vertices
	{
		{heart_vertices[0].position,normals[0],{ 0.f, 0.f }}, // 26
		{heart_vertices[13].position,normals[0],{ 1.f, 0.f }},
		{heart_vertices[14].position,normals[0],{ 0.5f, 1.f }},
		{heart_vertices[0].position,normals[0],{ 0.f, 0.f }},
		{heart_vertices[14].position,normals[0],{ 1.f, 0.f }},
		{heart_vertices[1].position,normals[0],{ 0.5f, 1.f }},

		{heart_vertices[1].position,normals[1],{ 0.f, 0.f }}, // 32
		{heart_vertices[14].position,normals[1],{ 1.f, 0.f }},
		{heart_vertices[15].position,normals[1],{ 0.5f, 1.f }},
		{heart_vertices[1].position,normals[1],{ 0.f, 0.f }},
		{heart_vertices[15].position,normals[1],{ 1.f, 0.f }},
		{heart_vertices[2].position,normals[1],{ 0.5f, 1.f }},

		{heart_vertices[2].position,normals[2],{ 0.f, 0.f }},	// 38
		{heart_vertices[15].position,normals[2],{ 1.f, 0.f }},
		{heart_vertices[16].position,normals[2],{ 0.5f, 1.f }},
		{heart_vertices[2].position,normals[2],{ 0.f, 0.f }},
		{heart_vertices[16].position,normals[2],{ 1.f, 0.f }},
		{heart_vertices[3].position,normals[2],{ 0.5f, 1.f }},

		{heart_vertices[5].position,normals[3],{ 0.f, 0.f }},	// 44
		{heart_vertices[18].position,normals[3],{ 1.f, 0.f }},
		{heart_vertices[13].position,normals[3],{ 0.5f, 1.f }},
		{heart_vertices[5].position,normals[3],{ 0.f, 0.f }},
		{heart_vertices[13].position,normals[3],{ 1.f, 0.f }},
		{heart_vertices[0].position,normals[3],{ 0.5f, 1.f }},

		{heart_vertices[6].position,normals[4],{ 0.f, 0.f }},	// 50
		{heart_vertices[19].position,normals[4],{ 1.f, 0.f }},
		{heart_vertices[20].position,normals[4],{ 0.5f, 1.f }},
		{heart_vertices[6].position,normals[4],{ 0.f, 0.f }},
		{heart_vertices[20].position,normals[4],{ 1.f, 0.f }},
		{heart_vertices[7].position,normals[4],{ 0.5f, 1.f }},

		{heart_vertices[7].position,normals[5],{ 0.f, 0.f }},	// 56
		{heart_vertices[20].position,normals[5],{ 1.f, 0.f }},
		{heart_vertices[21].position,normals[5],{ 0.5f, 1.f }},
		{heart_vertices[7].position,normals[5],{ 0.f, 0.f }},
		{heart_vertices[21].position,normals[5],{ 1.f, 0.f }},
		{heart_vertices[8].position,normals[5],{ 0.5f, 1.f }},

		{heart_vertices[8].position,normals[6],{ 0.f, 0.f }},
		{heart_vertices[21].position,normals[6],{ 1.f, 0.f }},
		{heart_vertices[22].position,normals[6],{ 0.5f, 1.f }},
		{heart_vertices[8].position,normals[6],{ 0.f, 0.f }},
		{heart_vertices[22].position,normals[6],{ 1.f, 0.f }},
		{heart_vertices[9].position,normals[6],{ 0.5f, 1.f }},

		{heart_vertices[9].position,normals[7],{ 0.f, 0.f }},
		{heart_vertices[22].position,normals[7],{ 1.f, 0.f }},
		{heart_vertices[23].position,normals[7],{ 0.5f, 1.f }},
		{heart_vertices[9].position,normals[7],{ 0.f, 0.f }},
		{heart_vertices[23].position,normals[7],{ 1.f, 0.f }},
		{heart_vertices[10].position,normals[7],{ 0.5f, 1.f }},

		{heart_vertices[12].position,normals[8],{ 0.f, 0.f }},
		{heart_vertices[25].position,normals[8],{ 1.f, 0.f }},
		{heart_vertices[18].position,normals[8],{ 0.5f, 1.f }},
		{heart_vertices[12].position,normals[8],{ 0.f, 0.f }},
		{heart_vertices[18].position,normals[8],{ 1.f, 0.f }},
		{heart_vertices[5].position,normals[8],{ 0.5f, 1.f }},

		{heart_vertices[12].position,normals[9],{ 0.f, 0.f }},
		{heart_vertices[23].position,normals[9],{ 1.f, 0.f }},
		{heart_vertices[25].position,normals[9],{ 0.5f, 1.f }},
		{heart_vertices[12].position,normals[9],{ 0.f, 0.f }},
		{heart_vertices[10].position,normals[9],{ 1.f, 0.f }},
		{heart_vertices[23].position,normals[9],{ 0.5f, 1.f }},
	};

	heart_vertices.insert(heart_vertices.end(), heart_side_vertices.begin(), heart_side_vertices.end());

	std::vector<uint32_t> heart_indices
	{
		0,1,2, 2,3,0, 3,4,5, 5,0,3,
		6,7,8, 8,9,6, 9,10,11, 11,6,9,
		12,5,4,	12,11,10,	12,4,11, 4,6,11,

		15,14,13, 13,16,15, 18,17,16, 16,13,18,
		21,20,19, 19,22,21, 24,23,22, 22,19,24,
		17,18,25,	23,14,25,	24,17,25, 24,19,17,

		26,27,28,
		29,30,31,
		32,33,34,
		35,36,37,
		38,39,40,

		41,42,43,
		44,45,46,
		47,48,49,
		50,51,52,
		53,54,55,

		56,57,58,
		59,60,61,
		62,63,64,
		65,66,67,
		68,69,70,

		71,72,73,
		74,75,76,
		77,78,79,
		80,81,82,
		83,84,85
	};

	m_heart_mesh = engine::mesh::create(heart_vertices, heart_indices);
}

engine::heart::~heart() {};

engine::ref<engine::heart> engine::heart::create(float size)
{
	return std::make_shared<engine::heart>(size);
}
