#include <pickup_coin.h>
#include "platform/opengl/gl_shader.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

pickup_coin::pickup_coin(){};
pickup_coin::~pickup_coin() {};
void pickup_coin::on_render()
{
	if (m_pickup_2->active())
	{
		auto mesh_shader = engine::renderer::shaders_library()->get("mesh");

		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
		m_pickup_2->textures().at(0)->bind();
		glm::mat4 pickup_transform(1.0f);
		pickup_transform = glm::translate(pickup_transform, m_pickup_2->position());
		pickup_transform = glm::rotate(pickup_transform, m_pickup_2->rotation_amount(), m_pickup_2->rotation_axis());
		engine::renderer::submit(mesh_shader, m_pickup_2->meshes().at(0), pickup_transform);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
	}
};

void pickup_coin::on_initialize()
{
	engine::ref<engine::coin> pickup_shape_2 = engine::coin::create();
	engine::ref<engine::texture_2d> pickup_texture_2 =
		engine::texture_2d::create("assets/textures/texture_gold_2.jpg", true);
	engine::game_object_properties pickup_props_2;
	pickup_props_2.position = { 0.f, 1.f, 5.f };
	pickup_props_2.meshes = { pickup_shape_2->mesh() };
	pickup_props_2.textures = { pickup_texture_2 };
	m_pickup_2 = pickup::create(pickup_props_2);
	m_pickup_2->init();
};

void pickup_coin::on_update(glm::vec3 c, float dt)
{
	m_pickup_2->update(c, dt);
};

