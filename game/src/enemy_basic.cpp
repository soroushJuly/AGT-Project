#include "enemy_basic.h"

enemy_basic::enemy_basic() : m_instantaneous_acceleration(0.f),
m_contact_time(0.f), m_attack_timer(0.f), m_damage_timer(0.f)
{
}

enemy_basic::~enemy_basic()
{
}

void enemy_basic::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, 1.f));
	m_object->set_velocity(glm::vec3(0.f, 0.f, 1.f));
	//m_object->set_acceleration(0.1f * glm::vec3(m_object->forward()));


	m_enemy_box.set_box(m_object->bounding_shape().x * m_object->scale().x,
		m_object->bounding_shape().y * m_object->scale().x,
		m_object->bounding_shape().z * m_object->scale().x,
		m_object->position());
	walk();

	m_billboard = billboard::create("assets/textures/hit.png", 4, 4, 16);

	std::srand(std::time(nullptr));
}
void enemy_basic::on_update(const engine::timestep& time_step, player& player, engine::bounding_box m_player_box, const glm::vec3& target_position)
{
	if (m_is_dead && m_timer < 0.1f)
	{
		return;
	}

	m_enemy_box.on_update(m_object->position());
	if (m_enemy_box.collision(m_player_box) && player.is_punching())
	{
		take_damage();
	}

	m_billboard->on_update(time_step);


	// Turn off instantaneous forces if contact time is surpassed
	if (glm::length(m_instantaneous_acceleration) > 0 && m_contact_time > 0.3f) {
		m_instantaneous_acceleration = glm::vec3(0.f);
		m_contact_time = 0.f;
	}
	m_contact_time += time_step;
	m_attack_timer += time_step;
	m_damage_timer += time_step;
	m_timer -= time_step;

	if (m_timer > 0.0f)
	{
		m_timer -= (float)time_step;
		if (m_timer < 0.0f)
		{
			m_object->animated_mesh()->switch_root_movement(false);
			m_object->animated_mesh()->switch_animation(2);
			m_timer = 0.0f;
		}
	}

	const float distance = glm::length(target_position - m_object->position());
	const float WALK_BOUND = 1.5f;
	const float RUN_BOUND = 5.f;

	switch (m_state)
	{
	case enemy_basic::WANDER:
		wander(time_step);
		if (distance < RUN_BOUND)
			m_state = enemy_basic::CHASE_RUN;
		if (distance < WALK_BOUND)
			m_state = enemy_basic::CHASE_WALK;
		break;
	case enemy_basic::CHASE_RUN:
		chase_enemy_run(time_step, target_position);
		if (distance < WALK_BOUND)
			m_state = enemy_basic::CHASE_WALK;
		if (distance > RUN_BOUND)
			m_state = enemy_basic::WANDER;
		break;
	case enemy_basic::CHASE_WALK:
		chase_enemy_walk(time_step, target_position);
		if (distance > WALK_BOUND)
			m_state = enemy_basic::CHASE_RUN;
		if (distance < .5f)
			m_state = enemy_basic::ATTACK;
		break;
	case enemy_basic::ATTACK:
		if (m_attack_timer > 1.f)
		{
			attack(time_step, player, m_player_box);
			m_attack_timer = 0.f;
		}
		if (distance > 0.5f)
			m_state = enemy_basic::CHASE_WALK;
		break;
	default:
		break;
	}
}

void enemy_basic::on_render(const engine::ref<engine::shader> mesh_shader, const engine::perspective_camera& camera)
{
	if (m_is_dead && m_timer < 0.1f)
	{
		return;
	}
	glm::mat4 object_transform(1.0f);
	object_transform = glm::translate(object_transform, m_object->position());
	object_transform = glm::rotate(object_transform, m_object->rotation_amount(), m_object->rotation_axis());
	object_transform = glm::scale(object_transform, glm::vec3(0.16f));
	engine::renderer::submit(mesh_shader, object_transform, m_object);

	m_enemy_box.on_render(2.5f, 1.f, 1.f, mesh_shader);

	m_billboard->on_render(camera, mesh_shader);
}

void enemy_basic::take_damage()
{
	if (m_damage_timer < 1.f)
	{
		return;
	}
	float x_position = m_object->position().x;
	float y_position = m_object->position().y + m_object->animated_mesh()->size().y * m_object->scale().y;
	float z_position = m_object->position().z;
	m_billboard->activate(glm::vec3(x_position, y_position, z_position), 2.f, 2.f);

	m_damage_timer = 0.f;
	m_instantaneous_acceleration = -(glm::normalize(glm::vec3(m_object->forward())) * 900.f) / m_object->mass();
	m_contact_time = 0.f;
	--m_health;
	if (m_health < 1)
	{
		die();
		m_is_dead = true;
	}
}

void enemy_basic::run()
{
	if (m_timer > 0.f)
	{
		return;
	}
	m_run_animation = 3;
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(m_run_animation);
	m_timer = glm::clamp((float)m_object->animated_mesh()->animations().at(m_run_animation)->mDuration, 0.f, 1.65f);
}

void enemy_basic::walk()
{
	if (m_timer > 0.f)
	{
		return;
	}
	// TODO: add walking animation for the skeleton
	m_walk_animation = 3;
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(m_walk_animation);
	m_timer = glm::clamp((float)m_object->animated_mesh()->animations().at(m_walk_animation)->mDuration, 0.f, 1.65f);
}

void enemy_basic::die()
{
	if (m_timer > 0.f)
	{
		return;
	}
	m_die_animation = 1;
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(m_die_animation);
	m_timer = glm::clamp((float)m_object->animated_mesh()->animations().at(m_die_animation)->mDuration, 0.f, .8f);
}

// TODO: take this functionality in a class
void enemy_basic::wander(const engine::timestep& time_step)
{
	walk();
	const float circle_distance = 16.f; // distance between particle and circle
	const float angle_step = .4f; // size of random angle step increment
	const float radius = 4.f;

	// set the circle CENTER position ahead of the particle
	glm::vec3 circle_position = m_object->position() + glm::normalize(m_object->velocity()) * circle_distance;

	// now we need to have an angle set, and increment it with some random every step
	float therand = ((float)(std::rand() % 99) - 50) / 100;
	m_angle += therand * angle_step;

	const float x_displace = radius * cos(m_angle);
	const float z_displace = radius * sin(m_angle);
	glm::vec3 wander_position = glm::vec3(circle_position.x + x_displace, m_object->position().y, circle_position.z + z_displace);

	glm::vec3 steering = glm::normalize(wander_position - m_object->position());

	// rotate the character to face the correct direction
	const float theta = acos(glm::dot(glm::normalize(m_object->velocity()), glm::normalize(m_object->forward())));
	if (m_object->velocity().x < 0)
		m_object->set_rotation_amount(-theta);
	else
		m_object->set_rotation_amount(theta);

	// limiting the speed
	glm::vec3 speed = m_object->velocity() + steering;
	if ((glm::length(speed)) > 0.3f)
		m_object->set_velocity(glm::normalize(speed) * .3f + (m_object->acceleration()) * (float)time_step);
	else
		m_object->set_velocity(speed + (m_object->acceleration()) * (float)time_step);

	// TODO: limiting the position
	glm::vec3 position = m_object->position() + m_object->velocity() * (float)time_step;
	m_object->set_position(position);
}

void enemy_basic::chase_enemy_run(const engine::timestep& time_step, const glm::vec3& target_position)
{
	m_speed = 2.f;
	run();
	chase_target(time_step, target_position);
}

void enemy_basic::chase_enemy_walk(const engine::timestep& time_step, const glm::vec3& target_position)
{
	m_speed = 0.5f;
	walk();
	chase_target(time_step, target_position);
}

void enemy_basic::attack(const engine::timestep& time_step, player& player, engine::bounding_box m_player_box)
{
	m_attack_animation = 0;
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(m_attack_animation);

	if (m_enemy_box.collision(m_player_box))
		player.take_damage(time_step);

	m_object->set_velocity(m_object->velocity() + m_instantaneous_acceleration * (float)time_step);
	m_object->set_position(m_object->position() + m_object->velocity() * (float)time_step);
}

void enemy_basic::chase_target(const engine::timestep& time_step, const glm::vec3& target_position)
{
	m_object->set_forward(target_position - m_object->position());
	m_object->set_velocity(glm::normalize(m_object->forward()) * m_speed + m_instantaneous_acceleration * (float)time_step);
	m_object->set_position(m_object->position() + m_object->velocity() * (float)time_step);
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}
