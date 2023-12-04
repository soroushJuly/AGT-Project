#include "pch.h"
#include "coin.h"
#include "engine.h"


engine::coin::coin() : m_size(0.02f), m_radius(0.1f)
{
	std::vector<engine::mesh::vertex> coin_vertices;
	std::vector<glm::vec2> texts_coordinates;
	const float repeat = 0.5f;
	const float repeat_sides = 0.2f;

	texts_coordinates =
	{
		glm::vec2(repeat * 1.0, repeat * 0.7),
		glm::vec2(repeat * 0.7, repeat * 1.0),
		glm::vec2(repeat * 0.3, repeat * 1.0),
		glm::vec2(repeat * 0.1, repeat * 0.7),
		glm::vec2(repeat * 0.1, repeat * 0.3),
		glm::vec2(repeat * 0.3, repeat * 0.1),
		glm::vec2(repeat * 0.7, repeat * 0.1),
		glm::vec2(repeat * 1.0, repeat * 0.3),
	};
	//	// position			normal			text coordinates
	// Front Octagon
	for (int i = 0; i < 8; i++)
	{
		float theta = i * (360 / 8) * (engine::PI / 180) + (engine::PI / 8);
		engine::mesh::vertex vertex(
			glm::vec3(m_radius * cos(theta), m_radius * sin(theta), m_size),
			glm::vec3(0.f, 0.f, -1.f),
			texts_coordinates[i]
		);

		coin_vertices.push_back(vertex);
	}
	// Back of the octagon
	for (int i = 0; i < 8; i++)
	{
		float theta = i * (360 / 8) * (engine::PI / 180) + (engine::PI / 8);
		engine::mesh::vertex vertex(
			glm::vec3(m_radius * cos(theta), m_radius * sin(theta), -1 * m_size),
			glm::vec3(0.f, 0.f, 1.f),
			texts_coordinates[i]
		);

		coin_vertices.push_back(vertex);
	}


	std::vector<glm::vec3> normals;
	// We need 8 surfaces
	normals.push_back(glm::cross(coin_vertices[0].position - coin_vertices[8].position, coin_vertices[9].position - coin_vertices[8].position));
	normals.push_back(glm::cross(coin_vertices[1].position - coin_vertices[9].position, coin_vertices[10].position - coin_vertices[9].position));
	normals.push_back(glm::cross(coin_vertices[2].position - coin_vertices[10].position, coin_vertices[11].position - coin_vertices[10].position));
	normals.push_back(glm::cross(coin_vertices[3].position - coin_vertices[11].position, coin_vertices[12].position - coin_vertices[11].position));
	normals.push_back(glm::cross(coin_vertices[4].position - coin_vertices[12].position, coin_vertices[13].position - coin_vertices[12].position));
	normals.push_back(glm::cross(coin_vertices[5].position - coin_vertices[13].position, coin_vertices[14].position - coin_vertices[13].position));
	normals.push_back(glm::cross(coin_vertices[6].position - coin_vertices[14].position, coin_vertices[15].position - coin_vertices[14].position));
	normals.push_back(glm::cross(coin_vertices[7].position - coin_vertices[15].position, coin_vertices[8].position - coin_vertices[15].position));


	std::vector<mesh::vertex> side_vertices
	{
		// defining the vertices in every surface between two octagon
		// postitions		normal		tex coord
		{coin_vertices[0].position,	normals[0], { 0.f, 0.f }},
		{coin_vertices[8].position,	normals[0], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[9].position,	normals[0], { repeat_sides * 0.5f,repeat_sides * 1.f }},

		{coin_vertices[0].position,	normals[0], { repeat_sides * 0.5f, repeat_sides * 1.f }},
		{coin_vertices[9].position,	normals[0], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[1].position,	normals[0], { 0.f, 0.f }},

		{coin_vertices[1].position,	normals[1], { 0.f, 0.f }},
		{coin_vertices[9].position,	normals[1], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[10].position,normals[1], { repeat_sides * 0.5f, repeat_sides * 1.f }},

		{coin_vertices[1].position,	normals[1], { repeat_sides * 0.5f, repeat_sides * 1.f }},
		{coin_vertices[10].position,normals[1], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[2].position,	normals[1], { 0.f, 0.f }},

		{coin_vertices[2].position,	normals[2], { 0.f, 0.f }},
		{coin_vertices[10].position,normals[2], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[11].position,normals[2], { repeat_sides * 0.5f, repeat_sides * 1.f }},

		{coin_vertices[2].position,	normals[2], { repeat_sides * 0.5f, repeat_sides * 1.f }},
		{coin_vertices[11].position,normals[2], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[3].position,	normals[2], { 0.f, 0.f }},


		{coin_vertices[3].position,	normals[3], { 0.f, 0.f }},
		{coin_vertices[11].position,normals[3], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[12].position,normals[3], { repeat_sides * 0.5f, repeat_sides * 1.f }},

		{coin_vertices[3].position,	normals[3], { repeat_sides * 0.5f, repeat_sides * 1.f }},
		{coin_vertices[12].position,normals[3], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[4].position,	normals[3], { 0.f, 0.f }},

		{coin_vertices[4].position,	normals[4], { 0.f, 0.f }},
		{coin_vertices[12].position,normals[4], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[13].position,normals[4], { repeat_sides * 0.5f, repeat_sides * 1.f }},

		{coin_vertices[4].position,	normals[4], { repeat_sides * 0.5f, repeat_sides * 1.f }},
		{coin_vertices[13].position,normals[4], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[5].position,	normals[4], { 0.f, 0.f }},

		{coin_vertices[5].position,	normals[5], { 0.f, 0.f }},
		{coin_vertices[13].position,normals[5], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[14].position,normals[5], { repeat_sides * 0.5f, repeat_sides * 1.f }},

		{coin_vertices[5].position,	normals[5], { repeat_sides * 0.5f, repeat_sides * 1.f }},
		{coin_vertices[14].position,normals[5], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[6].position,	normals[5], { 0.f, 0.f }},

		{coin_vertices[6].position,	normals[6], { 0.f, 0.f }},
		{coin_vertices[14].position,normals[6], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[15].position,normals[6], { repeat_sides * 0.5f, repeat_sides * 1.f }},

		{coin_vertices[6].position,	normals[6], { repeat_sides * 0.5f, repeat_sides * 1.f }},
		{coin_vertices[15].position,normals[6], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[7].position,	normals[6], { 0.f, 0.f }},

		{coin_vertices[7].position,	normals[7], { 0.f, 0.f }},
		{coin_vertices[15].position,normals[7], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[8].position,	normals[7], { repeat_sides * 0.5f, repeat_sides * 1.f }},

		{coin_vertices[7].position,	normals[7], { repeat_sides * 0.5f, repeat_sides * 1.f }},
		{coin_vertices[8].position,	normals[7], { repeat_sides * 1.f, 0.f }},
		{coin_vertices[0].position,	normals[7], { 0.f, 0.f }},
	};

	// add surface in between to the octagons
	coin_vertices.insert(coin_vertices.end(), side_vertices.begin(), side_vertices.end());

	const std::vector<uint32_t> coin_indices =
	{
		2,0,1,
		2,3,4,
		4,5,6,
		6,7,0,
		0,2,4,
		0,4,6,
		10,9,8,
		11,10,8,
		12,11,8,
		13,12,8,
		14,13,8,
		15,14,8,

		16,17,18,
		19,20,21,
		22,23,24,
		25,26,27,
		28,29,30,
		31,32,33,
		34,35,36,
		37,38,39,
		40,41,42,
		43,44,45,
		46,47,48,
		49,50,51,
		52,53,54,
		55,56,57,
		58,59,60,
		61,62,63,
	};

	m_coin_mesh = engine::mesh::create(coin_vertices, coin_indices);
}

engine::coin::~coin() {};

engine::ref<engine::coin> engine::coin::create()
{
	return std::make_shared<engine::coin>();
}
