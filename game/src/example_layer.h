#pragma once
#include <engine.h>
#include <player.h>
#include <enemy_basic.h>
#include <enemy_mech.h>
#include "enemy_lava.h"
#include "enemy_basic_skeleton.h"
#include "enemies/enemy_basic_robot.h"
#include "enemies/enemy_spike.h"
#include <game_intro.h>
#include "pickup_coin.h"
#include "pickup_heart.h"
#include "pickup_speed.h"
#include "decorations.h"
#include "engine/entities/bounding_box.h"
#include "engine/entities/shapes/heightmap.h"
#include "FX/cross_fade.h"
#include "FX/billboard.h"
#include "FX/ring.h"
#include "hud.h"


class pickup;
class pickup_coin;

class example_layer : public engine::layer
{
	enum game_state
	{
		MENU,
		IN_GAME,
		GAME_WON,
		GAME_LOST
	};

	int m_state = MENU;
public:
	example_layer();
	~example_layer();

	void on_update(const engine::timestep& time_step) override;
	void on_render() override;
	void on_event(engine::event& event) override;

private:
	player m_player{};
	enemy_mech m_enemy_mech{};

	std::vector<engine::ref<enemy_basic_skeleton>> m_skeleton_list;
	std::vector<engine::ref<enemy_basic_robot>> m_robot_list;
	std::vector<engine::ref<enemy_spike>> m_spike_list;


	enemy_lava m_lava_01;
	enemy_lava m_lava_02;
	enemy_lava m_lava_03;
	enemy_lava m_lava_04;
	enemy_lava m_lava_05;
	enemy_lava m_lava_06;

	pickup_speed m_pickup_speed_01{};
	pickup_heart m_pickup_heart_01{};
	pickup_heart m_pickup_heart_02{};
	std::vector<engine::ref<pickup_coin>> m_coin_list;

	decorations m_decorations;



	engine::ref<engine::game_object> arrow;
	engine::ref<engine::game_object> spike;

	engine::bounding_box m_player_box;
	engine::bounding_box m_skeleton_box;
	//engine::bounding_box m_lava_box;
	engine::bounding_box m_world_box_01;
	engine::bounding_box m_world_box_02;
	engine::bounding_box m_world_box_03;
	engine::bounding_box m_world_box_04;
	engine::bounding_box m_world_box_05;
	engine::bounding_box m_world_box_06;

	engine::ref<game_intro> m_game_intro;
	engine::ref<game_intro> m_game_won;
	engine::ref<game_intro> m_game_lost;
	hud hud;
	engine::timer m_play_time;
	float m_reached_time;
	float m_remained_time;

	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_terrain_2{};
	engine::ref<engine::heightmap>		m_heightmap;
	engine::ref<engine::game_object>	m_mannequin{};
	engine::ref<engine::game_object>	m_mech{};

	engine::ref<engine::material>		m_mannequin_material{};

	engine::ref<cross_fade>							m_cross_fade{};

	engine::DirectionalLight            m_directionalLight;
	engine::SpotLight m_spot_light;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	engine::ref<engine::text_manager>	m_text_manager{};

	engine::orthographic_camera       m_2d_camera;
	engine::perspective_camera        m_3d_camera;

	ring m_ring;

	float							  m_physical_terrain_height = 0.5f;
};
