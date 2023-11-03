#include "pch.h"
#include "coin.h"
#include "engine.h"


engine::coin::coin() : m_size(0.02f), m_radius(0.1f)
{
	std::vector<engine::mesh::vertex> coin_vertices;
	//	// position			normal			text coordinates
	// Front Octagon
	for (int i = 0; i < 8; i++)
	{
		float theta = i * (360 / 8) * (engine::PI / 180) + (engine::PI / 8);
		engine::mesh::vertex vertex(
			glm::vec3(m_radius * cos(theta), m_radius * sin(theta), m_size),
			glm::vec3(0.f, 0.f, -1.f),
			glm::vec2(cos(theta) / 2.f + 0.25f, sin(theta / 2.f) + 0.25f)
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
			glm::vec2(cos(theta) / 2.f + 0.5f, sin(theta / 2.f) + 0.5f)
		);

		coin_vertices.push_back(vertex);
	}

	// Inner Octagon
	//for (int i = 0; i < 8; i++)
	//{
	//	float theta = engine::PI * 180 * (i / 8.f);
	//	engine::mesh::vertex vertex(glm::vec3(cos(theta), sin(theta), 0.6f), glm::vec3(0.f, 0.f, -1.f), ());

	//	coin_vertices.push_back(vertex);
	//}

	std::vector<uint32_t> coin_indices =
	{
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,6,
		0,6,7,
		10,9,8,
		11,10,8,
		12,11,8,
		13,12,8,
		14,13,8,
		15,14,8,
		0,8,9,
		0,9,1,
		1,9,10,
		1,10,2,
		2,10,11,
		2,11,3,
		3,11,12,
		3,12,4,
		4,12,13,
		4,13,5,
		5,13,14,
		5,14,6,
		6,14,15,
		6,15,7,
		7,15,8,
		7,8,0
	};

	m_coin_mesh = engine::mesh::create(coin_vertices, coin_indices);
}

engine::coin::~coin() {};

engine::ref<engine::coin> engine::coin::create()
{
	return std::make_shared<engine::coin>();
}
