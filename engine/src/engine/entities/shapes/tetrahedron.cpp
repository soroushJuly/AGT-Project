#include "pch.h" 
#include "tetrahedron.h" 
#include <engine.h> 

engine::tetrahedron::tetrahedron(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;

	// We need 4 surfaces
	// so zero should be the top vertex of the tetrahedron
	// front
	normals.push_back(glm::cross(vertices[0] - vertices[2], vertices[0] - vertices[1]));
	// left
	normals.push_back(glm::cross(vertices[0] - vertices[3], vertices[0] - vertices[2]));
	// right
	normals.push_back(glm::cross(vertices[0] - vertices[1], vertices[0] - vertices[3]));
	// bottom
	normals.push_back(glm::cross(vertices[1] - vertices[2], vertices[1] - vertices[3]));

	std::vector<mesh::vertex> tetrahedron_vertices
	{
		// defining the vertices in every surface
		// postitions		normal		tex coord
		// front
		{vertices[0],	normals[0], { 0.f, 0.f }},
		{vertices[2],	normals[0], { 1.f, 0.f }},
		{vertices[1],	normals[0], { 0.5f, 1.f }},
		// left
		{vertices[0],	normals[1], { 0.f, 0.f }},
		{vertices[3],	normals[1], { 1.f, 0.f }},
		{vertices[2],	normals[1], { 0.5f, 1.f }},
		// right
		{vertices[0],	normals[2], { 0.f, 0.f }},
		{vertices[1],	normals[2], { 1.f, 0.f }},
		{vertices[3],	normals[2], { 0.5f, 1.f }},
		// bottom
		{vertices[1],	normals[3], { 0.f, 0.f }},
		{vertices[2],	normals[3], { 1.f, 0.f }},
		{vertices[3],	normals[3], { 0.5f, 1.f }},
	};

	std::vector<uint32_t> tetrahedron_indices
	{
		0,1,2,
		3,4,5,
		6,7,8,
		9,10,11
	};

	m_mesh = engine::mesh::create(tetrahedron_vertices, tetrahedron_indices);
}

engine::tetrahedron::~tetrahedron() {};

engine::ref<engine::tetrahedron> engine::tetrahedron::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::tetrahedron>(vertices);
}
