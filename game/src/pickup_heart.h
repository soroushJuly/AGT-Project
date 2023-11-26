#pragma once
#pragma once
#include <engine.h>
#include <pickup.h>

class player;

class pickup_heart
{
public:
	pickup_heart();
	~pickup_heart();

	void on_update(glm::vec3 c, int& heart, float dt, engine::ref<engine::audio_manager> audio = nullptr);
	void on_render();
	void on_initialize(glm::vec3 position = glm::vec3(0.f, 1.f, 0.f));

	engine::ref<engine::game_object> object() const { return m_heart; }

private:
	engine::ref<engine::game_object>	m_heart{};

	engine::ref<pickup> m_pickup{};

	glm::vec3 m_position;
	engine::ref<engine::game_object> m_ball;
	float theta;
	engine::PointLight m_pointLight;
	uint32_t num_point_lights = 1;
};
