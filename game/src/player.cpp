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
	if (engine::input::key_pressed(engine::key_codes::KEY_D)) // left
		turn(-1.0f * time_step);

}

void player::turn(float angle)
{
	m_object->set_forward(glm::rotate(m_object->forward(), angle, glm::vec3(0.f, 1.f,
		0.f)));
}
