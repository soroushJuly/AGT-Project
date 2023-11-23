#pragma once
#include <engine.h>
#include <pickup.h>

class pickup_coin
{
public:
	pickup_coin();
	~pickup_coin();

	void on_update(glm::vec3 c, int& coins, float dt, engine::ref<engine::audio_manager> audio = nullptr);
	void on_render();
	void on_initialize(glm::vec3 position = glm::vec3(0.f, 1.f, 0.f));

	engine::ref<engine::game_object> object() const { return m_coin; }

private:
	engine::ref<engine::game_object>	m_coin{};

	engine::ref<pickup> m_pickup{};

	glm::vec3 m_position;
};
