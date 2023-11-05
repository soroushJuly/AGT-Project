#pragma once
#include <engine.h>
#include <player.h>
#include "pickup_coin.h"
#include <static_object.h>

class pickup;
class pickup_coin;
class static_object;

class example_layer : public engine::layer
{
public:
	example_layer();
	~example_layer();

	void on_update(const engine::timestep& time_step) override;
	void on_render() override;
	void on_event(engine::event& event) override;

private:
	void check_bounce();
	player m_player{};
	pickup_coin m_pickup_coin{};
	pickup_coin m_pickup_coin_02{};
	pickup_coin m_pickup_coin_03{};
	pickup_coin m_pickup_coin_04{};
	pickup_coin m_pickup_coin_05{};
	pickup_coin m_pickup_coin_06{};
	pickup_coin m_pickup_coin_07{};
	static_object bush{};
	static_object tree_01{};
	static_object tree_02{};
	static_object tree_03{};
	static_object fence{};

	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_ball{};
	engine::ref<engine::game_object>	m_mannequin{};
	engine::ref<engine::game_object>	m_tetrahedron{};

	engine::ref<engine::material>		m_material{};
	engine::ref<engine::material>		m_tetrahedron_material{};
	engine::ref<engine::material>		m_mannequin_material{};

	engine::DirectionalLight            m_directionalLight;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	float								m_prev_sphere_y_vel = 0.f;
	engine::ref<engine::text_manager>	m_text_manager{};

	engine::orthographic_camera       m_2d_camera;
	engine::perspective_camera        m_3d_camera;

	engine::ref<pickup> m_pickup{};
	//engine::ref<pickup> m_pickup_2{};
};
