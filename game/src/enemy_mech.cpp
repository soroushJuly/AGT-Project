#include "enemy_mech.h"

enemy_mech::enemy_mech() : m_instantaneous_acceleration(0.f), m_contact_time(0.f)
{
}

enemy_mech::~enemy_mech()
{
}

void enemy_mech::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, 1.f));

	engine::ref <engine::model> model = engine::model::create("assets/models/static/Bomb.obj");
	std::vector<engine::ref<engine::texture_2d>> model_texture = { engine::texture_2d::create("assets/textures/bomb.png", false) };
	engine::game_object_properties model_props;
	model_props.meshes = model->meshes();
	model_props.velocity = glm::vec3(0.f);
	model_props.restitution = 0.9f;
	model_props.mass = 10.9f;
	model_props.textures = model_texture;
	model_props.bounding_shape = glm::vec3(.2f);
	model_props.position = m_object->position() + glm::vec3(0.f, m_object->animated_mesh()->size().y / 2, 0.f);
	model_props.scale = glm::vec3(.2f);
	m_bomb = engine::game_object::create(model_props);
	//m_object->set_acceleration(0.1f * glm::vec3(m_object->forward()));
	//walk();

}
void enemy_mech::on_update(const engine::timestep& time_step)
{
	m_object->animated_mesh()->on_update(time_step);
	update_bomb(time_step);
	//m_object->set_velocity(m_object->velocity() + (m_object->acceleration()
	//	+ m_instantaneous_acceleration) * (float)time_step);
	//m_object->set_position(m_object->position() + m_object->velocity() * (float)time_step);

	// Turn off instantaneous forces if contact time is surpassed
	if (glm::length(m_bomb_instantaneous_acceleration) > 0 && m_bomb_contact_time > 0.3f) {
		m_bomb_instantaneous_acceleration = glm::vec3(0.f);
		m_bomb_contact_time = 0.f;
	}
	m_bomb_contact_time += time_step;

	const float y_plane = 0.5f;
	if (m_bomb->position().y - m_bomb->bounding_shape().y < y_plane && m_bomb->velocity().y < 0)
	{
		LOG_INFO("ddd");
		float convergenceThreshold = 0.5f;
		if (glm::length(m_bomb->velocity()) > convergenceThreshold) {
			// The ball has bounced!  Implement a bounce by flipping the y velocity
			m_bomb->restitution();
			m_bomb->set_velocity(m_bomb->restitution() * glm::vec3(m_bomb->velocity()));
			//m_object->set_angular_velocity(m_object->restitution() * glm::vec3(m_object->angular_velocity()));
			m_bomb->set_velocity(glm::vec3(m_bomb->velocity().x, -m_bomb->velocity().y, m_bomb->velocity().z));
		}
		else {
			// Velocity of the ball is below a threshold.  Stop the ball. 
			m_bomb->set_velocity(glm::vec3(0.0f, 0.0f, 0.0f));
			m_bomb->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
			m_bomb->set_position(glm::vec3(m_bomb->position().x, m_bomb->bounding_shape().y + y_plane, m_bomb->position().z));
			//m_object->set_angular_velocity(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void enemy_mech::on_render(engine::ref<engine::shader> mesh_shader)
{
	if (is_bomb)
	{
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, m_bomb->position());
		//transform = glm::rotate(transform, -m_theta, glm::vec3(1.f, 0.f, 0.f));
		//transform = glm::rotate(transform, m_phi, glm::vec3(0.f, 1.f, 0.f));
		//transform = glm::rotate(transform, m_bomb->rotation_amount(), m_bomb->rotation_axis());
		engine::renderer::submit(mesh_shader, m_bomb);
	}
}

void enemy_mech::take_damage()
{

	m_instantaneous_acceleration = -(glm::normalize(glm::vec3(m_object->forward())) * 823.f) / m_object->mass();
	m_contact_time = 0.f;
	--m_health;
}

void enemy_mech::run()
{
	m_object->set_acceleration(0.5f * glm::vec3(m_object->forward()));
	max_velocity = 2.7;
}

void enemy_mech::walk()
{
	m_object->set_acceleration(0.1f * glm::normalize(glm::vec3(m_object->forward())));
	max_velocity = .9;
}

void enemy_mech::shoot_bomb()
{
	is_bomb = true;

	m_bomb->set_position(m_object->position() + glm::vec3(0.f, m_object->animated_mesh()->size().y / 2, 0.f));

	m_bomb->set_acceleration(glm::vec3(0.f, -9.8f, 0.f));
	m_bomb_contact_time = 0.f;
	m_bomb_instantaneous_acceleration = glm::vec3(20.f * m_object->forward());
}

void enemy_mech::update_bomb(const engine::timestep& time_step)
{
	m_bomb->set_velocity(m_bomb->velocity() + (m_bomb->acceleration() + m_bomb_instantaneous_acceleration) * (float)time_step);
	m_bomb->set_position(m_bomb->position() + m_bomb->velocity() * (float)time_step);
}

void enemy_mech::shoot_rocket()
{
}
