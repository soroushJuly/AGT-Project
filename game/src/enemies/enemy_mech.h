#pragma once
#include "engine.h"
#include "engine/entities/bounding_box.h"
#include "FX/billboard.h"
#include "player.h"

class player;

class enemy_mech
{
	enum states
	{
		IDLE,
		FACE_TARGET,
		ATTACK_BOMB,
		ATTACK_ROCKET
	};

	int m_state = IDLE;

public:
	enemy_mech();
	~enemy_mech();
	void initialise(engine::ref<engine::game_object> object);
	void on_update(const engine::timestep& time_step, player& player, engine::bounding_box m_player_box, glm::vec3 target_position);
	void on_render(engine::ref<engine::shader> mesh_shader, const engine::perspective_camera& camera);
	void take_damage();
	bool is_dead() { return m_is_dead; };

	// motions
	void idle();
	void walk();
	void shoot();
	void die();

	// state specific methods
	void patrol(const engine::timestep& time_step);
	void face_target(glm::vec3 target_position);
	void shoot_bomb(glm::vec3 target_position);
	void shoot_rocket();


	engine::ref<engine::game_object> object() const { return m_object; }
private:
	engine::ref< engine::game_object> m_object;
	int m_health{ 5 };
	float m_damage_timer;
	float m_timer;
	bool m_is_dead{ false };
	engine::bounding_box m_enemy_box;
	engine::ref<billboard>				m_billboard{};

	// control the patrol
	float m_default_time{ 5.f };
	float m_switch_direction_timer = m_default_time;

	bool is_walking{ false };
	float m_contact_time;
	glm::vec3 m_instantaneous_acceleration{ 0.f };

	// Bomb
	engine::ref<engine::game_object> m_bomb;
	engine::bounding_box m_bomb_box;
	bool is_bomb{ false };
	float m_bomb_contact_time;
	float m_bomb_timer;
	glm::vec3 m_bomb_instantaneous_acceleration{ 0.f };
	void update_bomb(const engine::timestep& time_step);

	// Rocket
	engine::ref<engine::game_object> m_rocket;
	engine::bounding_box m_rocket_box;
	bool is_rocket{ false };
	float m_rocket_max_velocity;
	float m_rocket_timer;
	void update_rocket(const engine::timestep& time_step, glm::vec3 target_position);

	//void start_damage_timer(const engine::timestep& time_step);
};

