#pragma once
#include <engine.h>

class quad;

class time_icon
{
public:
	time_icon(float width, float height);
	~time_icon();

	void on_update(const engine::timestep& time_step);
	void on_render(engine::ref<engine::shader> shader);

	static engine::ref<time_icon> create(float width, float height);

private:
	engine::ref<engine::texture_2d> m_texture;
	float m_transparency;
	engine::ref<quad> m_quad;

};
