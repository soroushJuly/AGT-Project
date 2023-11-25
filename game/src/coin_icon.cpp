#include "pch.h"
#include "coin_icon.h"
#include "quad.h"
#include "GLFW/include/GLFW/glfw3.h"


coin_icon::coin_icon(float width, float height) : m_step(0.0f)
{

	m_texture = engine::texture_2d::create("assets/textures/0.png", false);

	m_coin_icons.push_back(engine::texture_2d::create("assets/textures/coin_01.png", false));
	m_coin_icons.push_back(engine::texture_2d::create("assets/textures/coin_02.png", false));
	m_coin_icons.push_back(engine::texture_2d::create("assets/textures/coin_03.png", false));
	m_coin_icons.push_back(engine::texture_2d::create("assets/textures/coin_04.png", false));
	m_coin_icons.push_back(engine::texture_2d::create("assets/textures/coin_05.png", false));
	m_coin_icons.push_back(engine::texture_2d::create("assets/textures/coin_06.png", false));
	m_coin_icons.push_back(engine::texture_2d::create("assets/textures/coin_07.png", false));
	m_coin_icons.push_back(engine::texture_2d::create("assets/textures/coin_08.png", false));

	m_transparency = 1.f;

	m_quad = quad::create(glm::vec2(width, height));
}

coin_icon::~coin_icon()
{}

void coin_icon::on_update(const engine::timestep& time_step)
{
	m_step += 2.5 * time_step;

	if (m_step > 7.8)
	{
		m_step = 0.f;
	}
}

void coin_icon::on_render(engine::ref<engine::shader> shader)
{
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(-1.55f, .7f, .1f));
	transform = glm::scale(transform, glm::vec3(0.1f));


	// Turn off the lighting to have the original Intro screen
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", false);

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", m_transparency);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_coin_icons.at((int)m_step)->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);

	// Turn on the lighting to have the rest of objects in normal lighting
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", true);
}


engine::ref<coin_icon> coin_icon::create(float width, float height)
{
	return std::make_shared<coin_icon>(width, height);
}
