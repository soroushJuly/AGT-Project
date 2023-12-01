#include <pickup_coin.h>
#include "platform/opengl/gl_shader.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

pickup_coin::pickup_coin(glm::vec3 position) : m_position(position)
{
	engine::ref<engine::coin> pickup_shape = engine::coin::create();
	// Free texture from https://opengameart.org/content/gold-brick-1-lapisbrickbasecolorpng-0
	engine::ref<engine::texture_2d> pickup_texture =
		engine::texture_2d::create("assets/textures/texture_gold_3.png", false);
	engine::game_object_properties pickup_props;
	pickup_props.position = m_position;
	pickup_props.meshes = { pickup_shape->mesh() };
	pickup_props.textures = { pickup_texture };
	m_coin = engine::game_object::create(pickup_props);

	m_coin_box.set_box(0.15f, 0.15f, 0.15f, m_position);
};
pickup_coin::~pickup_coin() {};

void pickup_coin::on_initialize(glm::vec3 position)
{

};

engine::ref<pickup_coin> pickup_coin::create(glm::vec3 position)
{
	return std::make_shared<pickup_coin>(position);
}

void pickup_coin::on_update(glm::vec3 c, player& player, const engine::bounding_box player_box, float dt, const engine::ref<engine::audio_manager> m_audio_manager)
{
	m_coin->set_rotation_amount(m_coin->rotation_amount() + dt);
	if (m_coin_box.collision(player_box) && m_active)
	{
		m_active = false;
		if (m_audio_manager)
		{
			m_audio_manager->play("coin");
		}
		player.add_coin();
	}
};

void pickup_coin::on_render(const engine::ref<engine::shader> mesh_shader)
{
	if (m_active)
	{
		glm::mat4 pickup_transform(1.0f);
		pickup_transform = glm::translate(pickup_transform, m_position);
		pickup_transform = glm::rotate(pickup_transform, m_coin->rotation_amount(), m_coin->rotation_axis());
		engine::renderer::submit(mesh_shader, pickup_transform, m_coin);
	}
};

