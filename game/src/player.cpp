#include <pch.h>
#include <player.h>
// For getting input key codes:
#include "engine/core/input.h"
#include "engine/key_codes.h"
#include "FX/cross_fade.h"

player::player() : m_speed(0.f), m_timer(0.f), m_mouse_y(0.f), y_angle_y_mouse(0.f), x_angle_x_mouse(0.f),
m_damage_timer(0.f),
m_contact_time(0.f),
m_coins(0),
m_hearts(2)
{}
player::~player() {}

void player::initialise(engine::ref<engine::game_object> object, engine::ref<cross_fade> cross_fade, engine::ref<engine::audio_manager> audio_manager)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, 1.f));
	m_object->set_position(glm::vec3(0.f, 0.6f, 10.f));
	m_object->set_acceleration(glm::vec3(0.f, -9.8f, 0.f));
	m_object->set_velocity(glm::vec3(0.f, 0.f, 0.f));
	m_object->animated_mesh()->set_default_animation(4);

	m_audio_manager = audio_manager;
	m_cross_fade = cross_fade;

	default_bounding = m_object->bounding_shape();
}
void player::on_update(const engine::timestep& time_step)
{
	m_object->animated_mesh()->on_update(time_step);
	// x is where he looks at in the beginning and the z is the direction it walks to we want him to
	// be aligned with the walking direction
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
	m_damage_timer += time_step;

	if (glm::length(m_instantaneous_acceleration) > 0 && m_contact_time > 1.f) {
		m_instantaneous_acceleration = glm::vec3(0.f);
		m_contact_time = 0.f;
		//is_jumping = false;
	}

	m_object->set_velocity(m_object->velocity() + (m_object->acceleration() + m_instantaneous_acceleration) * (float)time_step);
	float x_position = m_object->position().x;
	float y_position = m_object->position().y;
	float z_position = m_object->position().z;
	//LOG_INFO("{}", m_object->acceleration() + m_instantaneous_acceleration);
	if (is_jumping)
	{
		m_object->set_velocity(m_object->velocity() + glm::normalize(m_object->forward()) * m_speed / 20.f * (float)time_step);
		m_object->set_position(glm::vec3(x_position, y_position, z_position) + m_object->velocity() * (float)time_step);
	}
	if (y_position < 0.5f && m_object->velocity().y < 0)
	{
		//m_object->set_acceleration(glm::vec3(0.0f, -9.8f, 0.0f));
		m_object->set_position(glm::vec3(m_object->position().x, 0.5f, m_object->position().z));
		clear_moves();
	}

	if ((is_walking || is_running) && !is_jumping)
	{
		LOG_INFO("last");
		m_object->set_velocity(glm::vec3(m_object->velocity().x, 0.f, m_object->velocity().z));
		m_object->set_position(glm::vec3(x_position, y_position, z_position) + m_object->velocity() * (float)time_step);
	}


	m_contact_time += time_step;

	if (engine::input::key_pressed(engine::key_codes::KEY_A)) // left
		turn(2.5f * time_step);
	if (engine::input::key_pressed(engine::key_codes::KEY_D)) // right
		turn(-2.5f * time_step);
	if (engine::input::key_pressed(engine::key_codes::KEY_SPACE) && engine::input::key_pressed(engine::key_codes::KEY_W))
	{
		jump(time_step);
	}


	if (engine::input::mouse_button_pressed(0))
	{
		punch(time_step);
	}
	else if (!m_is_punching && !is_jumping && engine::input::key_pressed(engine::key_codes::KEY_W) && engine::input::key_pressed(engine::key_codes::KEY_LEFT_SHIFT))
	{
		run(time_step);
	}
	else if (!m_is_punching && !is_jumping && engine::input::key_pressed(engine::key_codes::KEY_W))
	{
		walk(time_step);
	}

	if (m_timer > 0.0f)
	{
		m_timer -= (float)time_step;
		if (m_timer < 0.0f)
		{
			if (m_hearts < 1)
			{
				m_is_dead = true;
				return;
			}
			clear_moves();
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

void player::turn_back(const engine::timestep& time_step)
{
}

void player::jump(const engine::timestep& time_step)
{

	if (!is_jumping)
	{
		clear_moves();
		m_contact_time = 0.f;
		//const float force_watt = 1411.f;
		const float force_watt = 1011.f;
		float force = force_watt;
		float y_position = force * cos(engine::PI / 4);
		float x_position = glm::normalize(m_object->forward()).x * (force / 3) * cos(engine::PI / 4);
		float z_position = glm::normalize(m_object->forward()).z * (force / 3) * cos(engine::PI / 4);
		glm::vec3 jump_force = glm::vec3(x_position, y_position, z_position);
		m_instantaneous_acceleration = jump_force / m_object->mass();
		is_jumping = true;
		m_timer = 3.f;
	}

	m_object->animated_mesh()->switch_root_movement(false);
}

void player::walk(const engine::timestep& time_step)
{
	m_speed = 40.3f;
	m_object->set_velocity(glm::normalize(m_object->forward()) * m_speed * (float)time_step);
	m_instantaneous_acceleration = glm::vec3(0.f);

	if (m_timer > 0.0f && is_walking)
	{
		return;
	}
	clear_moves();
	is_walking = true;
	m_object->animated_mesh()->switch_animation(22);
	m_timer = glm::clamp((float)m_object->animated_mesh()->animations().at(22)->mDuration, 0.f, 1.65f);
}

void player::run(const engine::timestep& time_step)
{
	m_speed = 117.f;
	m_object->set_velocity(glm::normalize(m_object->forward()) * m_speed * (float)time_step);
	m_instantaneous_acceleration = glm::vec3(0.f);

	if (m_timer > 0.0f && is_running)
	{
		return;
	}
	clear_moves();
	is_running = true;
	m_object->animated_mesh()->switch_animation(16);
	m_timer = m_timer = glm::clamp((float)m_object->animated_mesh()->animations().at(16)->mDuration, 0.f, 2.f);

}

void player::punch(const engine::timestep& time_step)
{
	m_object->animated_mesh()->switch_root_movement(true);
	if (m_timer > 0.0f && m_is_punching)
	{

		return;
	}
	clear_moves();
	m_object->set_bounding_shape(m_object->bounding_shape() * 2.f);
	m_is_punching = true;
	m_object->animated_mesh()->switch_animation(14);
	m_timer = m_timer = glm::clamp((float)m_object->animated_mesh()->animations().at(14)->mDuration, 0.f, 1.f);

}

void player::clear_moves()
{
	m_object->set_velocity(glm::vec3(0.f, 0.f, 0.f));
	m_object->set_bounding_shape(default_bounding);
	m_is_punching = false;
	is_running = false;
	is_jumping = false;
	is_walking = false;
}

void player::update_camera(engine::perspective_camera& camera, const engine::timestep& time_step)
{
	auto [mouse_delta_x, mouse_delta_y] = engine::input::mouse_position();

	const float SENSITIVITY = 0.04f;
	const float CAMERA_DISTANCE_HEIGHT = 1.5;
	// Radius around the player for camera to rotate
	const float RADIUS = 3;

	// Camera's default position
	float camera_position_y = m_object->position().y + CAMERA_DISTANCE_HEIGHT;
	float camera_position_x = m_object->position().x + RADIUS * glm::normalize(m_object->forward()).x;
	float camera_position_z = m_object->position().z + RADIUS * glm::normalize(m_object->forward()).z;

	// the camera viewpoint
	float camera_look_x = m_object->position().x + glm::normalize(m_object->forward()).x;
	float camera_look_z = m_object->position().z + glm::normalize(m_object->forward()).z;
	glm::vec3 look_at = glm::vec3(camera_look_x, m_object->animated_mesh()->size().y / .5f, camera_look_z);

	// mouse movement in y direction
	y_angle_y_mouse = y_angle_y_mouse + mouse_delta_y * SENSITIVITY * time_step;
	m_mouse_y = RADIUS * sin(y_angle_y_mouse);
	if (m_mouse_y < -m_object->animated_mesh()->size().y / .5f + 0.1f)
		m_mouse_y = -m_object->animated_mesh()->size().y / .5f + 0.1f;
	if (m_mouse_y > m_object->animated_mesh()->size().y / .5f)
		m_mouse_y = m_object->animated_mesh()->size().y / .5f;

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

void player::take_damage(const engine::timestep& time_step)
{
	if (m_damage_timer > 1.5f)
	{
		m_damage_timer = 0;
		m_audio_manager->play("damage");
		m_cross_fade->activate();
		if (m_hearts < 1)
		{
			die();
			return;
		}
		m_hearts--;
	}
}

void player::die()
{
	m_object->animated_mesh()->switch_root_movement(true);
	if (m_timer > 0.0f)
	{

		return;
	}
	clear_moves();
	m_object->animated_mesh()->switch_animation(0);
	m_timer = m_timer = glm::clamp((float)m_object->animated_mesh()->animations().at(0)->mDuration, 0.f, 1.f);
}
