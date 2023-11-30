#pragma once
#include "engine.h"

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
	void on_update(const engine::timestep& time_step, glm::vec3 target_position);
	void on_render(engine::ref<engine::shader> mesh_shader);
	void take_damage();

	void run();
	void walk();

	void patrol(const engine::timestep& time_step);
	void face_target();
	void shoot_bomb();
	void shoot_rocket();


	engine::ref<engine::game_object> object() const { return m_object; }
private:
	engine::ref< engine::game_object> m_object;
	int m_health;
	float m_damage_timer;
	float m_speed;
	float m_timer;

	// control the patrol
	float m_default_time{ 4.f };
	float m_switch_direction_timer = m_default_time;

	float max_velocity;

	bool is_walking{ false };
	float m_contact_time;
	glm::vec3 m_instantaneous_acceleration{ 0.f };

	// Bomb
	engine::ref<engine::game_object> m_bomb;
	bool is_bomb{ false };
	float m_bomb_contact_time;
	float m_bomb_timer{ 3.f };
	glm::vec3 m_bomb_instantaneous_acceleration{ 0.f };
	void update_bomb(const engine::timestep& time_step);

	// Rocket
	engine::ref<engine::game_object> m_rocket;
	bool is_rocket{ false };
	float m_rocket_max_velocity;
	float m_rocket_timer{ 3.f };
	void update_rocket(const engine::timestep& time_step, glm::vec3 target_position);

	//void start_damage_timer(const engine::timestep& time_step);
};

