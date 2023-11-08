#include <pch.h>
#include <player.h>
// For getting input key codes:
#include "engine/core/input.h"
#include "engine/key_codes.h"


player::player() : m_speed(0.f), m_timer(0.f), m_mouse_y(0.f), y_angle_y_mouse(0.f), x_angle_x_mouse(0.f), m_coins(0)
{}
player::~player() {};

void player::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, -1.f));
	m_object->set_position(glm::vec3(0.f, 0.5, 10.f));
	m_object->animated_mesh()->set_default_animation(4);
}
void player::on_update(const engine::timestep& time_step)
{
	m_object->animated_mesh()->on_update(time_step);
	// x is where he looks at in the beginning and the z is the direction it walks to we want him to
	// be aligned with the walking direction
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
	if (is_stand_jumping)
	{
		stand_jump(time_step);
	}
	if (engine::input::key_pressed(engine::key_codes::KEY_A)) // left
		turn(1.0f * time_step);
	if (engine::input::key_pressed(engine::key_codes::KEY_D)) // right
		turn(-1.0f * time_step);
	if (engine::input::key_pressed(engine::key_codes::KEY_SPACE))
		stand_jump(time_step);
	// forward
	if (engine::input::key_pressed(engine::key_codes::KEY_W) && engine::input::key_pressed(engine::key_codes::KEY_LEFT_SHIFT))
	{
		run(time_step);
	}
	else if (engine::input::key_pressed(engine::key_codes::KEY_W))
	{
		walk(time_step);
	}

	//LOG_INFO("timer-1 [{}]", m_timer);
	if (m_timer > 0.0f)
	{
		m_timer -= (float)time_step;
		if (m_timer < 0.0f)
		{
			is_stand_jumping = false;
			m_object->animated_mesh()->switch_root_movement(false);
			m_object->animated_mesh()->switch_animation(m_object->animated_mesh()->default_animation());
			m_timer = 0.0f;
		}
	}
}

void player::turn(float angle)
{
	m_object->set_forward(glm::rotate(m_object->forward(), angle, glm::vec3(0.f, 1.f,
		0.f)));
}

void player::update_camera(engine::perspective_camera& camera, const engine::timestep& time_step)
{
	auto [mouse_delta_x, mouse_delta_y] = engine::input::mouse_position();

	const float SENSITIVITY = 0.04f;
	const float camera_distance_height = 1.5;
	// Radius around the player for camera to rotate
	const float RADIUS = 3;

	// Camera's default position
	float camera_position_y = m_object->position().y + camera_distance_height;
	float camera_position_x = m_object->position().x + RADIUS * glm::normalize(m_object->forward()).x;
	float camera_position_z = m_object->position().z + RADIUS * glm::normalize(m_object->forward()).z;

	// the camera viewpoint
	float camera_look_x = m_object->position().x + glm::normalize(m_object->forward()).x;
	float camera_look_z = m_object->position().z + glm::normalize(m_object->forward()).z;
	glm::vec3 look_at = glm::vec3(camera_look_x, m_object->animated_mesh()->size().y / 1, camera_look_z);

	// mouse movement in y direction
	y_angle_y_mouse = y_angle_y_mouse + mouse_delta_y * SENSITIVITY * time_step;
	m_mouse_y = RADIUS * sin(y_angle_y_mouse);
	if (m_mouse_y < -m_object->animated_mesh()->size().y / 1 + 0.1f)
		m_mouse_y = -m_object->animated_mesh()->size().y / 1 + 0.1f;
	if (m_mouse_y > m_object->animated_mesh()->size().y / 1)
		m_mouse_y = m_object->animated_mesh()->size().y / 1;

	// mouse movement in x direction
	x_angle_x_mouse = x_angle_x_mouse + mouse_delta_x * SENSITIVITY * time_step;
	glm::vec2 camera_dis_surface = glm::vec2(m_object->position().x - camera_position_x, m_object->position().z - camera_position_z);
	glm::vec2 camera_dis_rotated = glm::rotate<float>(camera_dis_surface, x_angle_x_mouse);
	camera_position_x = m_object->position().x + camera_dis_rotated.x;
	camera_position_z = m_object->position().z + camera_dis_rotated.y;

	camera.set_view_matrix(
		glm::vec3(camera_position_x, camera_position_y + m_mouse_y, camera_position_z),
		look_at);
}

void player::stand_jump(const engine::timestep& time_step)
{
	is_stand_jumping = true;
	//m_object->animated_mesh()->switch_root_movement(true);
	m_speed = 1.f;
	float x_position = m_object->position().x;
	float z_position = m_object->position().z;
	float y_position = m_object->position().y;
	if (m_timer > 1.f)
	{
		y_position += 1.f * m_speed * (float)time_step;
	}
	else
	{
		y_position = y_position - 1.f * m_speed * (float)time_step;
	}
	m_object->set_position(glm::vec3(x_position, y_position, z_position));
	if (m_timer > 0.0f)
	{
		return;
	}
	m_timer = 2.f;

}

void player::walk(const engine::timestep& time_step)
{
	//m_object->animated_mesh()->switch_root_movement(false);
	m_speed = 1.0f;
	m_object->set_position(m_object->position() += m_object->forward() * m_speed *
		(float)time_step);
	if (m_timer > 0.0f)
	{
		return;
	}
	m_object->animated_mesh()->switch_animation(22);
	m_timer = glm::clamp((float)m_object->animated_mesh()->animations().at(22)->mDuration, 0.f, 1.65f);
}

void player::run(const engine::timestep& time_step)
{
	m_object->animated_mesh()->switch_root_movement(true);
	m_speed = 3.0f;
	m_object->set_position(m_object->position() += m_object->forward() * m_speed *
		(float)time_step);
	if (m_timer > 0.0f)
	{
		return;
	}
	m_object->animated_mesh()->switch_animation(16);
	m_timer = m_timer = glm::clamp((float)m_object->animated_mesh()->animations().at(16)->mDuration, 0.f, 2.f);

}
