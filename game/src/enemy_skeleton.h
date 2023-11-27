#pragma once
#include "engine.h"

class enemy_skeleton
{
public:
	enemy_skeleton();
	~enemy_skeleton();
	void initialise(engine::ref<engine::game_object> object);
	void on_update(const engine::timestep& time_step);
	void take_damage();
	void run();
	void walk();


	engine::ref<engine::game_object> object() const { return m_object; }
private:
	int m_health;
	float m_damage_timer;
	float m_speed;
	float m_timer;

	float max_velocity;

	// probably should use these variables to control the character
	bool m_is_punching{ false };
	bool is_walking{ false }, is_running{ false }, is_standing{ false };
	engine::ref< engine::game_object> m_object;
	float m_contact_time;
	glm::vec3 m_instantaneous_acceleration{ 0.f };

	//void start_damage_timer(const engine::timestep& time_step);
};

