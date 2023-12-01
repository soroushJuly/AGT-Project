#pragma once
#pragma once
#include <engine.h>
#include <pickup.h>
#include "player.h"

class player;

class pickup_speed
{
public:
	pickup_speed();
	~pickup_speed();

	void on_update(glm::vec3 c, player& player, float dt, engine::ref<engine::audio_manager> audio = nullptr);
	void on_render();
	void on_initialize(glm::vec3 position = glm::vec3(0.f, 1.f, 0.f));

	engine::ref<engine::game_object> object() const { return m_arrow; }

private:
	engine::ref<engine::game_object>	m_arrow{};

	engine::ref<pickup> m_pickup{};

	glm::vec3 m_position;
	engine::ref<engine::game_object> m_light_emitter;
	engine::SpotLight m_spot_light;
	uint32_t num_spot_lights = 1;
};
