#include "pch.h"
#include "game_intro.h"
#include "quad.h"


game_intro::game_intro(const std::string& path, float width, float height)
{
	m_texture = engine::texture_2d::create(path, true);

	m_transparency = 1.f;

	m_quad = quad::create(glm::vec2(width, height));
	s_active = true;
}

game_intro::~game_intro()
{}

void game_intro::on_update(const engine::timestep& time_step)
{}

void game_intro::on_render(engine::ref<engine::shader> shader)
{
	if (!s_active)
		return;

	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(0.f, 0.f, 0.1f));

	// Turn off the lighting to have the original Intro screen
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

void game_intro::deactivate()
{
	s_active = false;
	m_transparency = 0.0f;
}

engine::ref<game_intro> game_intro::create(const std::string& path, float width, float height)
{
	return std::make_shared<game_intro>(path, width, height);
}
