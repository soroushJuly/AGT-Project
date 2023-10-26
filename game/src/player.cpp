#include <pch.h>
#include <player.h>
// For getting input key codes:
#include "engine/core/input.h"
#include "engine/key_codes.h"


player::player() : m_speed(1.f), m_timer(0.f)
{

}
player::~player() {};

void player::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, -1.f));
	m_object->set_position(glm::vec3(0.f, 0.5, 10.f));
	m_object->animated_mesh()->set_default_animation(1);
}
void player::on_update(const engine::timestep& time_step)
{
	m_object->set_position(m_object->position() += m_object->forward() * m_speed *
		(float)time_step);
	m_object->animated_mesh()->on_update(time_step);
	// x is where he looks at in the beginning and the z is the direction it walks to we want him to
	// be aligned with the walking direction
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
	if (engine::input::key_pressed(engine::key_codes::KEY_A)) // left
		turn(1.0f * time_step);
	if (engine::input::key_pressed(engine::key_codes::KEY_D)) // right
		turn(-1.0f * time_step);

}

void player::turn(float angle)
{
	m_object->set_forward(glm::rotate(m_object->forward(), angle, glm::vec3(0.f, 1.f,
		0.f)));
}

void player::update_camera(engine::perspective_camera& camera)
{
	const float looking_at = 6;
	const float camera_distance_height = 2;
	const float camera_distance_back = 3;
	// the camera position
	float height = m_object->position().y + camera_distance_height;
	float camera_position_x = m_object->position().x - camera_distance_back * m_object->forward().x;
	float camera_position_z = m_object->position().z - camera_distance_back * m_object->forward().z;
	// the camera viewpoint
	float camera_look_x = m_object->position().x + looking_at * m_object->forward().x ;
	float camera_look_z = m_object->position().z + looking_at * m_object->forward().z;
	//camera.position(glm::vec3(camera_x, height, camera_z));
	camera.set_view_matrix(glm::vec3(camera_position_x, height, camera_position_z), glm::vec3(camera_look_x, 0, camera_look_z));

}
