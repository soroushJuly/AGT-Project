#include "enemy_skeleton.h"

enemy_skeleton::enemy_skeleton() : m_instantaneous_acceleration(0.f), m_contact_time(0.f)
{
}

enemy_skeleton::~enemy_skeleton()
{
}

void enemy_skeleton::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	//m_object->set_acceleration(0.1f * glm::vec3(m_object->forward()));
	walk();
	//run();


}
void enemy_skeleton::on_update(const engine::timestep& time_step)
{
	m_object->set_velocity(m_object->velocity() + (m_object->acceleration()
		+ m_instantaneous_acceleration) * (float)time_step);
	m_object->set_position(m_object->position() + m_object->velocity() * (float)time_step);
	if (glm::length(m_object->velocity()) > max_velocity)
	{
		LOG_INFO("{}", m_object->velocity());
		m_object->set_velocity(max_velocity * glm::normalize(m_object->forward()));
	}

	// Turn off instantaneous forces if contact time is surpassed
	if (glm::length(m_instantaneous_acceleration) > 0 && m_contact_time > 0.3f) {
		m_instantaneous_acceleration = glm::vec3(0.f);
		m_contact_time = 0.f;
	}
	m_contact_time += time_step;
}

void enemy_skeleton::take_damage()
{

	m_instantaneous_acceleration = -(glm::normalize(glm::vec3(m_object->forward())) * 823.f) / m_object->mass();
	m_contact_time = 0.f;
	--m_health;
}

void enemy_skeleton::run()
{
	m_object->set_acceleration(0.5f * glm::vec3(m_object->forward()));
	max_velocity = 2.7;
}

void enemy_skeleton::walk()
{
	m_object->set_acceleration(0.1f * glm::normalize(glm::vec3(m_object->forward())));
	max_velocity = .9;
}
