#pragma once
#include <engine.h>
#include <pickup.h>
#include "engine/entities/bounding_box.h"
#include "player.h"

class player;

class pickup_coin
{
public:
	pickup_coin(glm::vec3 position);
	~pickup_coin();

	void on_update(glm::vec3 c, player& player,const engine::bounding_box player_box, float dt,const engine::ref<engine::audio_manager> audio = nullptr);
	void on_render(const engine::ref<engine::shader> mesh_shader);
	void on_initialize(glm::vec3 position = glm::vec3(0.f, 1.f, 0.f));

	static engine::ref<pickup_coin> create(glm::vec3 position);

	engine::ref<engine::game_object> object() const { return m_coin; }

private:
	engine::ref<engine::game_object>	m_coin{};
	engine::bounding_box m_coin_box;
	bool m_active{ true };

	glm::vec3 m_position;
};
