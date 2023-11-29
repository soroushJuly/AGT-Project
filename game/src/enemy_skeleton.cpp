#include "enemy_skeleton.h"

enemy_skeleton::enemy_skeleton() : m_instantaneous_acceleration(0.f), m_contact_time(0.f), m_attack_timer(0.f)
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
void enemy_skeleton::on_update(const engine::timestep& time_step, const glm::vec3& target_position)
{
	//m_object->set_velocity(m_object->velocity() + (m_object->acceleration()
	//	+ m_instantaneous_acceleration) * (float)time_step);
	//m_object->set_position(m_object->position() + m_object->velocity() * (float)time_step);
	//if (glm::length(m_object->velocity()) > max_velocity)
	//{
	//	m_object->set_velocity(max_velocity * glm::normalize(m_object->forward()));
	//}

	// Turn off instantaneous forces if contact time is surpassed
	if (glm::length(m_instantaneous_acceleration) > 0 && m_contact_time > 0.3f) {
		m_instantaneous_acceleration = glm::vec3(0.f);
		m_contact_time = 0.f;
	}
	m_contact_time += time_step;
	m_attack_timer += time_step;

	float distance = glm::length(target_position - m_object->position());
	const float WALK_BOUND = 1.5f;
	const float RUN_BOUND = 5.f;

	switch (m_state)
	{
	case enemy_skeleton::WANDER:
		wander(time_step);
		if (distance < RUN_BOUND)
			m_state = enemy_skeleton::CHASE_RUN;
		if (distance < WALK_BOUND)
			m_state = enemy_skeleton::CHASE_WALK;
		break;
	case enemy_skeleton::CHASE_RUN:
		chase_enemy_run(time_step, target_position);
		if (distance < WALK_BOUND)
			m_state = enemy_skeleton::CHASE_WALK;
		if (distance > RUN_BOUND)
			m_state = enemy_skeleton::WANDER;
		break;
	case enemy_skeleton::CHASE_WALK:
		chase_enemy_walk(time_step, target_position);
		if (distance > WALK_BOUND)
			m_state = enemy_skeleton::CHASE_RUN;
		if (distance < .5f)
			m_state = enemy_skeleton::ATTACK;
		break;
	case enemy_skeleton::ATTACK:
		if (m_attack_timer > 1.f)
		{
			attack();
			m_attack_timer = 0.f;
		}
		if (distance > 0.5f)
			m_state = enemy_skeleton::CHASE_WALK;
		break;
	default:
		break;
	}
}

void enemy_skeleton::take_damage()
{

	m_instantaneous_acceleration = -(glm::normalize(glm::vec3(m_object->forward())) * 823.f) / m_object->mass();
	m_contact_time = 0.f;
	--m_health;
}

void enemy_skeleton::run()
{
	m_run_animation = 3;
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(m_run_animation);
	//m_object->set_acceleration(0.5f * glm::vec3(m_object->forward()));
}

void enemy_skeleton::walk()
{
	m_walk_animation = 3;
	m_object->animated_mesh()->switch_animation(m_walk_animation);
	//m_object->set_acceleration(0.1f * glm::normalize(glm::vec3(m_object->forward())));
}

void enemy_skeleton::wander(const engine::timestep& time_step)
{
	walk();
	//m_object->set_position(m_object->position() + m_object->forward() * m_object->velocity() *
	//	(float)time_step);
	//m_object->set_forward(target_position - m_object->position());
}

void enemy_skeleton::chase_enemy_run(const engine::timestep& time_step, const glm::vec3& target_position)
{
	//run();
	//max_velocity = 2.7f;
	m_speed = 2.f;
	run();
	chase_target(time_step, target_position);
}

void enemy_skeleton::chase_enemy_walk(const engine::timestep& time_step, const glm::vec3& target_position)
{
	m_speed = 0.5f;
	chase_target(time_step, target_position);
	//walk();
}

void enemy_skeleton::attack()
{
	m_attack_animation = 0;
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(m_attack_animation);
}

void enemy_skeleton::chase_target(const engine::timestep& time_step, const glm::vec3& target_position)
{
	m_object->set_forward(target_position - m_object->position());
	m_object->set_velocity(glm::normalize(m_object->forward()) * m_speed + m_instantaneous_acceleration * (float)time_step);
	m_object->set_position(m_object->position() + m_object->velocity() * (float)time_step);
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}
