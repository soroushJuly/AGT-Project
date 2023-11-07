#pragma once
#include <engine.h>

class quad;

class game_intro
{
public:
	game_intro(const std::string& path, float width, float height);
	~game_intro();

	void on_update(const engine::timestep& time_step);
	void on_render(engine::ref<engine::shader> shader);
	void deactivate();

	static engine::ref<game_intro> create(const std::string& path, float width, float height);

private:
	bool s_active;

	float m_timer;
	float m_max_time;

	engine::ref<engine::texture_2d> m_texture;
	float m_transparency;
	engine::ref<quad> m_quad;
};
