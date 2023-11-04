#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"

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
	void jump();
	void walk(const engine::timestep& time_step);
	void run(const engine::timestep& time_step);
private:
	float m_speed;
	float m_timer;
	float m_mouse_y;
	double y_angle_y_mouse;
	double x_angle_x_mouse;

	// probably should use these variables to control the character
	bool is_walking{ false }, is_running{ false }, is_jumping{ false }, is_standing{ false };
	engine::ref< engine::game_object> m_object;
};
