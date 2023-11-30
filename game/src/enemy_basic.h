#pragma once
#include "engine.h"

class enemy_basic
{

	enum states
	{
		WANDER,
		CHASE_RUN,
		CHASE_WALK,
		ATTACK
	};

	int m_state = WANDER;
public:
	enemy_basic();
	~enemy_basic();

	void initialise(engine::ref<engine::game_object> object);
	void on_update(const engine::timestep& time_step, const glm::vec3& target_position);
	void take_damage();
	void run();
	void walk();


	// states related methods
	void wander(const engine::timestep& time_step);
	void chase_enemy_run(const engine::timestep& time_step, const glm::vec3& target_position);
	void chase_enemy_walk(const engine::timestep& time_step, const glm::vec3& target_position);
	void attack(const engine::timestep& time_step);
	void chase_target(const engine::timestep& time_step, const glm::vec3& target_position);


	engine::ref<engine::game_object> object() const { return m_object; }
private:
	int m_health;
	float m_damage_timer;
	float m_speed;
	float m_timer;
	float m_angle{ 0.f };

	float m_attack_timer;

	int m_run_animation;
	int m_walk_animation;
	int m_attack_animation;

	float max_velocity;

	// probably should use these variables to control the character
	bool m_is_punching{ false };
	bool is_walking{ false }, is_running{ false }, is_standing{ false };
	engine::ref< engine::game_object> m_object;
	float m_contact_time;
	glm::vec3 m_instantaneous_acceleration{ 0.f };

	//void start_damage_timer(const engine::timestep& time_step);
};

