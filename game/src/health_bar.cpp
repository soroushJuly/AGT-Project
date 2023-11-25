#include "pch.h"
#include "health_bar.h"
#include "quad.h"
#include "GLFW/include/GLFW/glfw3.h"


health_bar::health_bar(float width, float height)
{

	m_texture = engine::texture_2d::create("assets/textures/0.png", false);

	m_health_bars.push_back(engine::texture_2d::create("assets/textures/0.png", false));
	m_health_bars.push_back(engine::texture_2d::create("assets/textures/1.png", false));
	m_health_bars.push_back(engine::texture_2d::create("assets/textures/2.png", false));
	m_health_bars.push_back(engine::texture_2d::create("assets/textures/3.png", false));

	m_transparency = 1.f;

	m_quad = quad::create(glm::vec2(width, height));
}

health_bar::~health_bar()
{}

void health_bar::on_update(const engine::timestep& time_step)
{}

void health_bar::on_render(engine::ref<engine::shader> shader)
{
	//LOG_INFO("{}", (float)engine::application::window().width());
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(-1.4f, .83f, .1f));
	transform = glm::scale(transform, glm::vec3(0.35f));


	// Turn off the lighting to have the original Intro screen
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", false);

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", m_transparency);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_health_bars.at(3)->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);

	// Turn on the lighting to have the rest of objects in normal lighting
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("lighting_on", true);
}


engine::ref<health_bar> health_bar::create(float width, float height)
{
	return std::make_shared<health_bar>(width, height);
}
