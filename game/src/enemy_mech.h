#pragma once
#include "engine.h"

class enemy_mech
{
public:
	enemy_mech();
	~enemy_mech();
	void initialise(engine::ref<engine::game_object> object);
	void on_update(const engine::timestep& time_step);
	void on_render(engine::ref<engine::shader> mesh_shader);
	void take_damage();

	void run();
	void walk();

	void shoot_bomb();
	void shoot_rocket();


	engine::ref<engine::game_object> object() const { return m_object; }
private:
	int m_health;
	float m_damage_timer;
	float m_speed;
	float m_timer;
	engine::ref< engine::game_object> m_object;

	float max_velocity;

	bool is_walking{ false }, is_running{ false }, is_standing{ false };
	float m_contact_time;
	glm::vec3 m_instantaneous_acceleration{ 0.f };

	engine::ref<engine::game_object> m_bomb;
	bool is_bomb{ false };
	float m_bomb_contact_time;
	float m_bomb_timer{ 3.f };
	glm::vec3 m_bomb_instantaneous_acceleration{ 0.f };
	void update_bomb(const engine::timestep& time_step);


	//void start_damage_timer(const engine::timestep& time_step);
};

