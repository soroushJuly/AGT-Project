#include <pickup_heart.h>
#include "platform/opengl/gl_shader.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

pickup_heart::pickup_heart() : theta(0.f) {};
pickup_heart::~pickup_heart() {};

void pickup_heart::on_initialize(glm::vec3 position)
{
	m_position = position;
	engine::ref<engine::heart> pickup_shape = engine::heart::create();
	//engine::ref<engine::texture_2d> pickup_texture =
	//	engine::texture_2d::create("assets/textures/texture_gold_3.png", false);
	engine::game_object_properties pickup_props;
	pickup_props.position = position;
	pickup_props.scale = glm::vec3(.05f);
	pickup_props.meshes = { pickup_shape->mesh() };
	//pickup_props.textures = { pickup_texture };
	m_pickup = pickup::create(pickup_props);
	m_pickup->init();

	m_pointLight.Color = glm::vec3(1.0f, 0.5f, 0.5f);
	m_pointLight.AmbientIntensity = 0.1f;
	m_pointLight.DiffuseIntensity = 0.1f;
	m_pointLight.Position = m_pickup->position();

	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(20, 40, 0.04f);
	engine::game_object_properties sphere_props;
	sphere_props.meshes = { sphere_shape->mesh() };
	sphere_props.type = 1;
	sphere_props.bounding_shape = glm::vec3(0.5f);
	sphere_props.restitution = 0.92f;
	sphere_props.mass = 0.000001f;
	m_ball = engine::game_object::create(sphere_props);
};

void pickup_heart::on_render()
{
	auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
	if (m_pickup->active())
	{
		// point light moves around the object
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->
			set_uniform("gNumPointLights", (int)num_point_lights);
		m_pointLight.submit(mesh_shader, 0);


		engine::ref<engine::material> m_lightsource_material = engine::material::create(.7f, m_pointLight.Color,
			m_pointLight.Color, m_pointLight.Color, 0.4f);

		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->
			set_uniform("lighting_on", false);
		m_lightsource_material->submit(mesh_shader);
		engine::renderer::submit(mesh_shader, m_ball->meshes().at(0),
			glm::translate(glm::mat4(1.f), m_pointLight.Position));
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->
			set_uniform("lighting_on", true);


		engine::ref<engine::material> m_mannequin_material = engine::material::create(1.0f, glm::vec3(1.f, 0.1f, 0.1f),
			glm::vec3(1.f, 0.1f, 0.1f), glm::vec3(1.f, 0.1f, 0.1f), 1.0f);
		//std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
		m_mannequin_material->submit(mesh_shader);
		//m_pickup->textures().at(0)->bind();
		glm::mat4 pickup_transform(1.0f);
		pickup_transform = glm::translate(pickup_transform, m_position);
		pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
		pickup_transform = glm::scale(pickup_transform, m_pickup->scale());
		engine::renderer::submit(mesh_shader, m_pickup->meshes().at(0), pickup_transform);
		//std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
	}
	else
	{
		m_pointLight.Color = glm::vec3(0.f, 0.f, 0.f);
		m_pointLight.submit(mesh_shader, 0);
	}
};

void pickup_heart::on_update(glm::vec3 c, int& hearts, float dt, engine::ref<engine::audio_manager> m_audio_manager)
{

	theta += 2.5 * dt;
	const float radius = 2.f * glm::length(m_pickup->scale());
	m_pointLight.Position = glm::vec3(m_pickup->position().x + cos(theta) * radius,
		m_pickup->position().y + glm::length(m_pickup->scale()),
		m_pickup->position().z + sin(theta) * radius
	);

	if (m_pickup->update(c, dt))
	{
		if (m_audio_manager)
		{
			m_audio_manager->play("pickup");
		}
		if (hearts <= 3)
		{
			++hearts;
		}
		//auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
		//std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->
		//	set_uniform("gNumPointLights", (int)0);
	}
};

