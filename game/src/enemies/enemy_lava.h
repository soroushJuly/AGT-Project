#pragma once
#include "engine.h"
#include "engine/entities/bounding_box.h"

class enemy_lava
{
public:
	enemy_lava();
	~enemy_lava();

	void on_initialise(glm::vec3 position, glm::vec3 half_extends, float rotation = 0.f);
	void on_render(const engine::ref<engine::shader> mesh_shader);

	bool collision(engine::bounding_box other_box);

private:
	engine::ref<engine::game_object>	m_lava{};
	engine::bounding_box m_lava_box;
};


