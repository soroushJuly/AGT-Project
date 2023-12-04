#include "enemy_spike.h"
#include "platform/opengl/gl_shader.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

enemy_spike::enemy_spike(glm::vec3 position, float move_limit, float rotation) : m_position(position),
m_move_limit(move_limit),
m_rotation(rotation)
{
	// initiate spike
	// TODO: make it 4 spikes together to transfer the sense of the trap
	engine::game_object_properties spike_props;
	engine::ref<engine::spike> spike_shape = engine::spike::create(1.f);
	std::vector<engine::ref<engine::texture_2d>> spike_textures = { engine::texture_2d::create("assets/textures/metal.png", false) };
	spike_props.meshes = { spike_shape->mesh() };
	spike_props.textures = { spike_textures };
	spike_props.position = position;
	spike_props.scale = glm::vec3(.5f);
	spike_props.bounding_shape = spike_props.scale;
	m_object = engine::game_object::create(spike_props);

	m_object->set_velocity(5.f * glm::vec3(cos(m_rotation), 0.f, sin(m_rotation)));

	m_spike_box.set_box(m_object->scale().x, m_object->scale().y * 2.f, m_object->scale().z, m_position);
};

enemy_spike::~enemy_spike() {};

void enemy_spike::on_update(const engine::timestep& time_step, player& player, const engine::bounding_box player_box)
{
	if (glm::length(m_position - object()->position()) > m_move_limit)
	{
		m_object->set_velocity(-1.f * m_object->velocity());
	}
	m_object->set_position(m_object->position() + m_object->velocity() * (float)time_step);
	m_spike_box.on_update(m_object->position());

	if (m_spike_box.collision(player_box))
		player.take_damage(time_step);
};

void enemy_spike::on_render(const engine::ref<engine::shader> mesh_shader)
{
	glm::mat4 transform(1.f);
	transform = glm::rotate(transform, m_rotation, glm::vec3(0.f, 1.f, 0.f));
	transform = glm::scale(transform, m_object->scale());
	engine::renderer::submit(mesh_shader, m_object);
};

engine::ref<enemy_spike> enemy_spike::create(glm::vec3 position, float move_limit, float rotation)
{
	return std::make_shared<enemy_spike>(position, move_limit, rotation);
}

