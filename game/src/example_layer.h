#pragma once
#include <engine.h>
#include <player.h>
#include <game_intro.h>
#include "pickup_coin.h"
#include "pickup_heart.h"
#include "pickup_speed.h"
#include <static_object.h>
#include "decorations.h"
#include "engine/entities/bounding_box.h"
#include "engine/entities/shapes/heightmap.h"
#include "health_bar.h"
#include "coin_icon.h"
#include "time_icon.h"
#include "FX/cross_fade.h"


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
	player m_player{};
	pickup_speed m_pickup_speed_01{};
	pickup_heart m_pickup_heart_01{};
	pickup_heart m_pickup_heart_02{};
	pickup_coin m_pickup_coin_01{};
	pickup_coin m_pickup_coin_02{};
	pickup_coin m_pickup_coin_03{};
	pickup_coin m_pickup_coin_04{};
	pickup_coin m_pickup_coin_05{};
	pickup_coin m_pickup_coin_06{};
	pickup_coin m_pickup_coin_07{};
	decorations m_decorations;
	static_object bush{};
	static_object campfire{};
	static_object tree_01{};
	static_object tree_02{};
	static_object tree_03{};
	static_object fence{};
	engine::ref<engine::game_object> arrow;
	engine::ref<engine::game_object> spike;

	engine::bounding_box m_player_box;
	engine::bounding_box m_lava_box;
	engine::bounding_box m_world_box_01;
	engine::bounding_box m_world_box_02;

	engine::ref<game_intro> m_game_intro;
	engine::ref<health_bar> m_health_bar;
	engine::ref<coin_icon> m_coin_icon;
	engine::ref<time_icon> m_time_icon;
	engine::timer m_play_time;

	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_terrain_2{};
	engine::ref<engine::heightmap>		m_heightmap;
	engine::ref<engine::game_object>	m_mannequin{};
	engine::ref<engine::game_object>	m_skeleton{};
	engine::ref<engine::game_object>	m_lava{};

	engine::ref<engine::material>		m_mannequin_material{};

	engine::ref<cross_fade>							m_cross_fade{};;

	engine::DirectionalLight            m_directionalLight;
	engine::SpotLight m_spot_light;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	engine::ref<engine::text_manager>	m_text_manager{};

	engine::orthographic_camera       m_2d_camera;
	engine::perspective_camera        m_3d_camera;

	bool is_intro_active;
	float							  m_physical_terrain_height = 0.5f;
};
