#include "pch.h"
#include "time_icon.h"
#include "quad.h"
#include "GLFW/include/GLFW/glfw3.h"


time_icon::time_icon(float width, float height)
{

	m_texture = engine::texture_2d::create("assets/textures/hourglass.png", false);

	m_transparency = 1.f;

	m_quad = quad::create(glm::vec2(width, height));
}

time_icon::~time_icon()
{}

void time_icon::on_update(const engine::timestep& time_step)
{}

void time_icon::on_render(engine::ref<engine::shader> shader)
{
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(-1.545f, .585f, .1f));
	transform = glm::scale(transform, glm::vec3(0.15f));


	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", m_transparency);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_texture->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);

	// Turn on the lighting to have the rest of objects in normal lighting
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", true);
}


engine::ref<time_icon> time_icon::create(float width, float height)
{
	return std::make_shared<time_icon>(width, height);
}
