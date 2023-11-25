#pragma once
#include <engine.h>

class quad;

class health_bar
{
public:
	health_bar(float width, float height);
	~health_bar();

	void on_update(const engine::timestep& time_step);
	void on_render(engine::ref<engine::shader> shader);

	static engine::ref<health_bar> create(float width, float height);

private:
	engine::ref<engine::texture_2d> m_texture;
	float m_transparency;
	engine::ref<quad> m_quad;
	std::vector<engine::ref<engine::texture_2d>> m_health_bars;

};
