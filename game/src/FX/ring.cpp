#include "pch.h"
#include "ring.h"


ring::ring()
{
	s_active = false;
	m_radius = 2.5f;
	m_start_radius = m_radius;
}

ring::~ring()
{}

void ring::on_update(const engine::timestep& timestep, glm::vec3 position)
{
	if (s_active == false)
		return;

	m_position = position;
	m_position += glm::vec3(0.f, 0.01f, 0.f);
	// Life decreasing on each iteration
	m_life -= 0.5f * (float)timestep;
	if (m_life < 0) {
		s_active = false;
		return;
	}

	float f = cos(m_life * 2.75f); // Nonlinear mapping
	m_radius += .3f * f * (float)timestep;
	if (m_radius < 0.22f)
	{
		m_radius = 0.22f;
	}
}

void ring::initialise()
{
	// Load the texture
	m_texture = engine::texture_2d::create("assets/textures/ring.png", false);

	glm::vec3 x_axis = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 z_axis = glm::vec3(0.f, 0.f, 1.f);

	// Assume oriented in xz plane
	glm::vec3 normal = glm::vec3(0.f, 1.f, 0.f);

	// Determine the coordinates of the quad	   	 
	std::vector<engine::mesh::vertex> quad_vertices
	{
		//							 position												normal		 tex coord       
		{ glm::vec3(-m_start_radius * x_axis - m_start_radius * z_axis),		normal,		{ 0.f, 0.f } },
		{ glm::vec3(-m_start_radius * x_axis + m_start_radius * z_axis),		normal,		{ 0.f, 1.f } },
		{ glm::vec3(m_start_radius * x_axis + m_start_radius * z_axis),		normal,		{ 1.f, 1.f } },
		{ glm::vec3(m_start_radius * x_axis - m_start_radius * z_axis),		normal,		{ 1.f, 0.f } },
	};

	const std::vector<uint32_t> quad_indices
	{
		 0,  1,  2,
		 0,  2,  3,
	};

	m_quad = engine::mesh::create(quad_vertices, quad_indices);
}

void ring::activate(float radius, glm::vec3 position)
{
	m_position = position;
	m_life = 5.0f;
	m_radius = radius;
	s_active = true;

}

void ring::on_render(engine::ref<engine::shader> shader)
{

	if (s_active == false)
		return;



	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_texture->bind();
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position);
	transform = glm::scale(transform, glm::vec3(m_radius / m_start_radius));
	engine::renderer::submit(shader, m_quad, transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", true);

}
