#include "hud.h"

hud::hud()
{
}

hud::~hud()
{
}

void hud::on_initialize()
{
	m_health_bar = health_bar::create(0.54f, 0.17f);
	m_coin_icon = coin_icon::create(0.39f, 0.48f);
	m_time_icon = time_icon::create(0.32f, 0.32f);
}

void hud::on_update(const engine::timestep& time_step, int hearts)
{
	m_health_bar->on_update(hearts);
	m_coin_icon->on_update(time_step);
}

void hud::on_render(engine::ref<engine::shader> mesh_shader)
{
	m_health_bar->on_render(mesh_shader);
	m_coin_icon->on_render(mesh_shader);
	m_time_icon->on_render(mesh_shader);
}
