#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"

class cross_fade;

class player
{
public:
	player();
	~player();
	void initialise(engine::ref<engine::game_object> object);
	void on_update(const engine::timestep& time_step);

	engine::ref<engine::game_object> object() const { return m_object; }

	void turn(float angle);
	void update_camera(engine::perspective_camera& camera, const engine::timestep& timestep);
	void stand_jump(const engine::timestep& time_step);
	void jump(const engine::timestep& time_step);
	void walk(const engine::timestep& time_step);
	void punch(const engine::timestep& time_step);
	void run(const engine::timestep& time_step);
	void turn_back(const engine::timestep& time_step);
	void clear_moves();
	void die();
	void take_damage(engine::ref<engine::audio_manager> audio_manager, engine::ref<cross_fade> cross_fade, const engine::timestep& timestep);

	glm::vec3 position() { return m_object->position(); };
	int& coins() { return m_coins; }
	int& hearts() { return m_hearts; }
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

	// probably should use these variables to control the character
	bool is_turned_back{ false }, is_jumping{ false }, m_is_punching{ false };
	bool is_walking{ false }, is_running{ false }, is_stand_jumping{ false }, is_standing{ false };
	engine::ref< engine::game_object> m_object;

	void start_damage_timer(const engine::timestep& time_step);
};
