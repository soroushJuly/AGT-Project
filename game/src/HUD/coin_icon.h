#pragma once
#include <engine.h>

class quad;

class coin_icon
{
public:
	coin_icon(float width, float height);
	~coin_icon();

	void on_update(const engine::timestep& time_step);
	void on_render(engine::ref<engine::shader> shader);

	static engine::ref<coin_icon> create(float width, float height);

private:
	engine::ref<engine::texture_2d> m_texture;
	float m_transparency;
	engine::ref<quad> m_quad;
	std::vector<engine::ref<engine::texture_2d>> m_coin_icons;
	float m_step;
	//int timer

};
