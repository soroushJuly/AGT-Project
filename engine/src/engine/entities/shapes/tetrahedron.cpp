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
		// postitions		normal		tex coord
		// front
		{},
		{},
		{},
		// left
		{},
		{},
		{},
		// right
		// bottom

	};
}
