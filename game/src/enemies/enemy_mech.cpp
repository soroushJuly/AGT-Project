#include "enemy_mech.h"

enemy_mech::enemy_mech() : m_instantaneous_acceleration(0.f), m_damage_timer(0.f),
m_bomb_contact_time(0.f), m_timer(0.f), m_bomb_timer{ 3.f }, m_rocket_timer{ 3.f },
m_contact_time(0.f), m_rocket_max_velocity(0.f), m_switch_direction_timer(0.f)
{
}

enemy_mech::~enemy_mech()
{
}

void enemy_mech::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, 1.f));

	m_enemy_box.set_box(m_object->animated_mesh()->size().x * m_object->scale().x,
		m_object->animated_mesh()->size().y * m_object->scale().y,
		m_object->animated_mesh()->size().z * m_object->scale().z,
		m_object->position());

	m_billboard = billboard::create("assets/textures/hit.png", 4, 4, 16);

	// Bomb
	engine::ref <engine::model> model = engine::model::create("assets/models/static/Bomb.obj");
	std::vector<engine::ref<engine::texture_2d>> model_texture = { engine::texture_2d::create("assets/textures/bomb.png", false) };
	engine::game_object_properties model_props;
	model_props.meshes = model->meshes();
	model_props.velocity = glm::vec3(0.f);
	model_props.restitution = 0.9f;
	model_props.mass = 10.9f;
	model_props.textures = model_texture;
	model_props.bounding_shape = glm::vec3(.3f);
	model_props.position = m_object->position() + glm::vec3(0.f, m_object->animated_mesh()->size().y / 2, 0.f);
	model_props.scale = glm::vec3(.2f);
	m_bomb = engine::game_object::create(model_props);
	m_bomb_box.set_box(m_bomb->scale().x, m_bomb->scale().y, m_bomb->scale().z,
		m_bomb->position());

	// rocket
	engine::ref <engine::model> rocket_model = engine::model::create("assets/models/static/Rocket.fbx");
	engine::game_object_properties rocket_model_props;
	rocket_model_props.meshes = rocket_model->meshes();
	rocket_model_props.velocity = glm::vec3(0.f);
	rocket_model_props.mass = 60.9f;
	rocket_model_props.textures = { rocket_model->textures() };
	rocket_model_props.bounding_shape = glm::vec3(.4f);
	rocket_model_props.position = m_object->position() + glm::vec3(0.f, m_object->animated_mesh()->size().y / 2, 0.f);
	rocket_model_props.scale = glm::vec3(.005f);
	m_rocket = engine::game_object::create(rocket_model_props);
	m_rocket->set_forward(m_object->forward());
	m_rocket_box.set_box(m_rocket->scale().x, m_rocket->scale().y, m_rocket->scale().z,
		m_rocket->position());
}
void enemy_mech::on_update(const engine::timestep& time_step, player& player, engine::bounding_box m_player_box, glm::vec3 target_position)
{
	m_object->animated_mesh()->on_update(time_step);
	update_bomb(time_step);
	update_rocket(time_step, player.object()->position());
	m_billboard->on_update(time_step);

	m_enemy_box.on_update(m_object->position());
	m_bomb_box.on_update(m_bomb->position());
	m_rocket_box.on_update(m_rocket->position());

	if (m_enemy_box.collision(m_player_box) && player.is_punching())
		take_damage();
	if (m_bomb_box.collision(m_player_box))
	{
		player.take_damage(time_step);
		is_bomb = false;
	}
	if (m_rocket_box.collision(m_player_box))
	{
		is_rocket = false;
		player.take_damage(time_step);
		m_rocket->set_position(m_object->position());
	}
	if (m_rocket->position().y < 0.7)
		is_rocket = false;


	// Turn off instantaneous forces if contact time is surpassed
	if (glm::length(m_bomb_instantaneous_acceleration) > 0 && m_bomb_contact_time > 0.3f) {
		m_bomb_instantaneous_acceleration = glm::vec3(0.f);
		m_bomb_contact_time = 0.f;
	}
	m_bomb_contact_time += time_step;
	m_bomb_timer += time_step;
	m_rocket_timer += time_step;
	m_damage_timer += time_step;
	m_timer -= time_step;

	if (m_timer > 0.0f)
	{
		m_timer -= (float)time_step;
		if (m_timer < 0.0f)
		{
			if (m_health < 0)
			{
				//m_object->set_scale(glm::vec3(0.0001f));
				m_is_dead = true;
			}
			m_object->animated_mesh()->switch_root_movement(false);
			m_object->animated_mesh()->switch_animation(2);
			m_timer = 0.0f;
		}
	}

	const float y_plane = 0.5f;
	if (m_bomb->position().y - m_bomb->bounding_shape().y < y_plane && m_bomb->velocity().y < 0)
	{

		// Check if the Velocity of the bomb is below a threshold. Then decide bounce or stop
		float convergenceThreshold = 0.6f;
		if (glm::length(m_bomb->velocity()) > convergenceThreshold) {
			m_bomb->restitution();
			m_bomb->set_velocity(m_bomb->restitution() * glm::vec3(m_bomb->velocity()));
			m_bomb->set_velocity(glm::vec3(m_bomb->velocity().x, -m_bomb->velocity().y, m_bomb->velocity().z));
		}
		else {
			m_bomb->set_velocity(glm::vec3(0.0f, 0.0f, 0.0f));
			m_bomb->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
			m_bomb->set_position(glm::vec3(m_bomb->position().x, m_bomb->bounding_shape().y + y_plane, m_bomb->position().z));
		}
	}

	const float distance = glm::length(target_position - m_object->position());
	const float FACE_BOUND = 13.f;
	const float BOMB_BOUND = 8.2f;
	const float ROCKET_BOUND = 3.2f;

	// TODO: shockwave attack to send the target(enemy) backwards
	switch (m_state)
	{
	case enemy_mech::IDLE:
		patrol(time_step);
		if (distance < FACE_BOUND)
			m_state = enemy_mech::FACE_TARGET;
		break;
	case enemy_mech::FACE_TARGET:
		face_target(target_position);
		if (distance < BOMB_BOUND)
			m_state = enemy_mech::ATTACK_BOMB;
		if (distance > FACE_BOUND)
			m_state = enemy_mech::IDLE;
		break;
	case enemy_mech::ATTACK_BOMB:
		face_target(target_position);
		if (m_bomb_timer > 3.f)
		{
			shoot_bomb(target_position);
			m_bomb_timer = 0.f;
		}
		if (distance < ROCKET_BOUND)
		{
			m_rocket_timer = 0.f;
			m_state = enemy_mech::ATTACK_ROCKET;
		}
		if (distance > BOMB_BOUND)
			m_state = enemy_mech::FACE_TARGET;
		break;
	case enemy_mech::ATTACK_ROCKET:
		face_target(target_position);
		if (m_rocket_timer > 2.2f)
		{
			shoot_rocket();
			m_rocket_timer = 0.f;
		}
		if (distance > ROCKET_BOUND)
		{
			m_state = enemy_mech::ATTACK_BOMB;
		}
		break;
	default:
		break;
	}
}

void enemy_mech::on_render(engine::ref<engine::shader> mesh_shader, const engine::perspective_camera& camera)
{
	glm::mat4 transform_mech(1.0f);
	transform_mech = glm::translate(transform_mech, m_object->position());
	transform_mech = glm::scale(transform_mech, m_object->scale());
	transform_mech = glm::rotate(transform_mech, m_object->rotation_amount(), glm::vec3(0.f, 1.f, 0.f));
	engine::renderer::submit(mesh_shader, transform_mech, m_object);

	m_billboard->on_render(camera, mesh_shader);

	if (is_bomb)
	{
		glm::mat4 transform(1.0f);
		//transform = glm::rotate(transform, -m_theta, glm::vec3(1.f, 0.f, 0.f));
		//transform = glm::rotate(transform, m_phi, glm::vec3(0.f, 1.f, 0.f));
		//transform = glm::rotate(transform, m_bomb->rotation_amount(), m_bomb->rotation_axis());
		engine::renderer::submit(mesh_shader, m_bomb);
	}

	if (is_rocket)
	{
		glm::mat4 transform_01(1.0f);
		transform_01 = glm::translate(transform_01, m_rocket->position());
		transform_01 = glm::scale(transform_01, m_rocket->scale());
		transform_01 = glm::rotate(transform_01, m_rocket->rotation_amount(), glm::vec3(0.f, 1.f, 0.f));
		engine::renderer::submit(mesh_shader, transform_01, m_rocket);
	}
}

void enemy_mech::take_damage()
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

	--m_health;
	if (m_health < 1)
		die();
}

void enemy_mech::shoot_bomb(glm::vec3 target_position)
{
	is_bomb = true;
	shoot();

	// TODO: add rotational movement to the bomb
	m_bomb->set_position(m_object->position() + glm::vec3(0.f, m_object->animated_mesh()->size().y / 2, 0.f));

	m_bomb->set_acceleration(glm::vec3(0.f, -9.8f, 0.f));
	m_bomb_contact_time = 0.f;

	m_bomb_instantaneous_acceleration = glm::vec3(20.f * glm::normalize(glm::vec3(target_position - m_object->position())));
}

void enemy_mech::update_bomb(const engine::timestep& time_step)
{
	m_bomb->set_velocity(m_bomb->velocity() + (m_bomb->acceleration() + m_bomb_instantaneous_acceleration) * (float)time_step);
	m_bomb->set_position(m_bomb->position() + m_bomb->velocity() * (float)time_step);
}

void enemy_mech::shoot_rocket()
{
	is_rocket = true;
	shoot();

	m_rocket->set_position(m_object->position() + glm::vec3(0.f, m_object->animated_mesh()->size().y / 2, 0.f));
	m_rocket_max_velocity = 2.8f;

	// rocket has a force to overcome gravity => a-rocktet > a-gravity 
	m_rocket->set_acceleration(glm::vec3(0, -3.55f, 0.f));
	m_rocket->set_velocity(m_object->forward());
}

void enemy_mech::update_rocket(const engine::timestep& time_step, glm::vec3 target_position)
{
	glm::vec3 target_position_2d = glm::vec3(target_position.x, m_rocket->position().y, target_position.z);
	glm::vec3 desired_path = target_position_2d - m_rocket->position();
	glm::vec3 desired_velocity = glm::normalize(desired_path) * m_rocket_max_velocity;
	glm::vec3 steering = desired_velocity - m_rocket->velocity();

	m_rocket->set_velocity(m_rocket->velocity() + steering + (m_rocket->acceleration()) * (float)time_step);

	float theta = acos(glm::dot(glm::normalize(m_rocket->forward()), glm::normalize(glm::vec3(target_position_2d - m_rocket->position()))));
	if (glm::vec3(target_position_2d - m_rocket->position()).x < 0)
	{
		m_rocket->set_rotation_amount(-theta);
	}
	else
	{
		m_rocket->set_rotation_amount(theta);
	}
	m_rocket->set_position(m_rocket->position() + m_rocket->velocity() * (float)time_step);
}

// move forwards until the timer runs out, then switch direction to move the other way
void enemy_mech::patrol(const engine::timestep& time_step)
{
	walk();
	m_switch_direction_timer -= (float)time_step;
	if (m_switch_direction_timer < 0.f)
	{
		m_object->set_forward(m_object->forward() * -1.f);
		m_switch_direction_timer = m_default_time;
	}

	m_object->set_position(m_object->position() + glm::normalize(m_object->forward()) * 1.f * (float)time_step);
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}

void enemy_mech::face_target(glm::vec3 target_position)
{
	idle();
	m_object->set_forward(target_position - m_object->position());
	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));
}

void enemy_mech::idle()
{
	if (m_timer > 0.f)
	{
		return;
	}
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(5);
	m_timer = glm::clamp((float)m_object->animated_mesh()->animations().at(5)->mDuration, 0.f, 1.65f);
}

void enemy_mech::walk()
{
	if (m_timer > 0.f)
	{
		return;
	}
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(15);
	m_timer = glm::clamp((float)m_object->animated_mesh()->animations().at(15)->mDuration, 0.f, 1.65f);
}

void enemy_mech::shoot()
{
	if (m_timer > 0.f)
	{
		return;
	}
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(13);
	m_timer = glm::clamp((float)m_object->animated_mesh()->animations().at(13)->mDuration, 0.f, 1.f);
}

void enemy_mech::die()
{
	if (m_timer > 0.f)
	{
		return;
	}
	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(1);
	m_timer = glm::clamp((float)m_object->animated_mesh()->animations().at(1)->mDuration, 0.f, 2.f);
}