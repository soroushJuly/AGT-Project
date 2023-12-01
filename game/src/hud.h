#pragma once
#include <engine.h>

#include "HUD/coin_icon.h"
#include "HUD/health_bar.h"
#include "HUD/time_icon.h"

class hud
{
public:
	hud();
	~hud();

	void on_initialize();
	void on_update(const engine::timestep& time_step, int hearts);
	void on_render(engine::ref<engine::shader> shader);

private:

	engine::ref<health_bar> m_health_bar;
	engine::ref<coin_icon> m_coin_icon;
	engine::ref<time_icon> m_time_icon;

};
