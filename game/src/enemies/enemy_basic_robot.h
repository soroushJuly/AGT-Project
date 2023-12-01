#pragma once
#include <engine.h>
#include "engine/entities/bounding_box.h"
#include "player.h"
#include "enemy_basic.h"

class player;

class enemy_basic_robot
{
public:
	enemy_basic_robot(glm::vec3 position);
	~enemy_basic_robot();

	void on_update(const engine::timestep& time_step, player& player, const engine::bounding_box player_box);
	void on_render(const engine::ref<engine::shader> mesh_shader, const engine::perspective_camera& camera);
	void on_initialize();

	static engine::ref<enemy_basic_robot> create(glm::vec3 position);

	engine::ref<engine::game_object> object() const { return m_object; }

private:
	engine::ref<engine::game_object>	m_object{};
	enemy_basic m_enemy_basic{};
	bool m_active{ true };

	glm::vec3 m_position;
};
