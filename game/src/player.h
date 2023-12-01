#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"
#include "FX/cross_fade.h"

class cross_fade;

class player
{
public:
	player();
	~player();
	void initialise(engine::ref<engine::game_object> object, engine::ref<cross_fade> cross_fade, engine::ref<engine::audio_manager> audio_manager);
	void on_update(const engine::timestep& time_step);

	engine::ref<engine::game_object> object() const { return m_object; }

	void turn(float angle);
	void update_camera(engine::perspective_camera& camera, const engine::timestep& timestep);
	void jump(const engine::timestep& time_step);
	void walk(const engine::timestep& time_step);
	void punch(const engine::timestep& time_step);
	void run(const engine::timestep& time_step);
	void turn_back(const engine::timestep& time_step);
	void clear_moves();
	void die();
	void take_damage(const engine::timestep& timestep);
	void add_heart() { ++m_hearts; };
	void add_coin() { ++m_coins; };

	glm::vec3 position() { return m_object->position(); };
	int coins() const { return m_coins; }
	int hearts() const { return m_hearts; }
	bool is_dead() const { return m_is_dead; }
	float& speed() { return m_speed; }
	bool is_punching() { return m_is_punching; }
private:
	int m_coins;
	int m_hearts;
	float m_damage_timer;
	float m_speed;
	float m_timer;
	float m_mouse_y;
	double y_angle_y_mouse;
	double x_angle_x_mouse;

	engine::ref<engine::audio_manager> m_audio_manager;
	engine::ref<cross_fade> m_cross_fade;

	glm::vec3 default_bounding{ 0.f };
	glm::vec3 m_instantaneous_acceleration{ 0.f };
	float m_contact_time;

	// probably should use these variables to control the character
	bool m_is_dead{ false };
	bool is_turned_back{ false }, is_jumping{ false }, m_is_punching{ false };
	bool is_walking{ false }, is_running{ false }, is_stand_jumping{ false }, is_standing{ false };
	engine::ref< engine::game_object> m_object;

	void start_damage_timer(const engine::timestep& time_step);
};
