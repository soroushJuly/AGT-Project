#include <pickup_coin.h>
#include "platform/opengl/gl_shader.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

pickup_coin::pickup_coin() {};
pickup_coin::~pickup_coin() {};

void pickup_coin::on_initialize(glm::vec3 position)
{
	m_position = position;
	engine::ref<engine::coin> pickup_shape = engine::coin::create();
	engine::ref<engine::texture_2d> pickup_texture =
		engine::texture_2d::create("assets/textures/texture_gold_3.png", false);
	engine::game_object_properties pickup_props;
	pickup_props.position = position;
	pickup_props.meshes = { pickup_shape->mesh() };
	pickup_props.textures = { pickup_texture };
	m_pickup = pickup::create(pickup_props);
	m_pickup->init();
};

void pickup_coin::on_render()
{
	if (m_pickup->active())
	{
		auto mesh_shader = engine::renderer::shaders_library()->get("mesh");

		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
		m_pickup->textures().at(0)->bind();
		glm::mat4 pickup_transform(1.0f);
		pickup_transform = glm::translate(pickup_transform, m_position);
		pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
		engine::renderer::submit(mesh_shader, m_pickup->meshes().at(0), pickup_transform);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
	}
};

void pickup_coin::on_update(glm::vec3 c, int& coins, float dt)
{
	if (m_pickup->update(c, dt))
	{
		++coins;
	}
	
};

