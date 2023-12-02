#pragma once
#include <engine.h>
#include "engine/entities/bounding_box.h"
#include "player.h"

class player;

class enemy_spike
{
public:
	enemy_spike(glm::vec3 center_position, float move_limit, float rotation);
	~enemy_spike();

	void on_update(const engine::timestep& time_step, player& player, const engine::bounding_box player_box);
	void on_render(const engine::ref<engine::shader> mesh_shader);

	static engine::ref<enemy_spike> create(glm::vec3 position, float move_limit, float rotation);

	engine::ref<engine::game_object> object() const { return m_object; }

private:
	engine::ref<engine::game_object>	m_object{};
	engine::bounding_box m_spike_box;
	glm::vec3 m_move_direction = glm::vec3(1.f, 0.f, 1.f);
	float m_move_limit;
	float m_rotation;
	glm::vec3 m_position;

	bool m_active{ true };
};
