#include <pickup_speed.h>
#include "platform/opengl/gl_shader.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

pickup_speed::pickup_speed() {};
pickup_speed::~pickup_speed() {};

void pickup_speed::on_initialize(glm::vec3 position)
{
	m_position = position;
	engine::ref<engine::arrow> pickup_shape = engine::arrow::create();
	engine::ref<engine::texture_2d> pickup_texture =
		engine::texture_2d::create("assets/textures/speed.jpg", false);
	engine::game_object_properties pickup_props;
	pickup_props.position = position;
	pickup_props.scale = glm::vec3(.2f);
	pickup_props.meshes = { pickup_shape->mesh() };
	pickup_props.textures = { pickup_texture };
	m_pickup = pickup::create(pickup_props);
	m_pickup->init();

	m_spot_light.Color = glm::vec3(.3f, 0.9f, 0.5f);
	m_spot_light.AmbientIntensity = 0.3f;
	m_spot_light.DiffuseIntensity = 0.3f;
	m_spot_light.Position = m_pickup->position() + glm::vec3(0.f, 1.7f * m_pickup->scale().y, 0.f);
	m_spot_light.Direction = glm::vec3(0.f, -1.f, 0.f);
	m_spot_light.Cutoff = 0.4f;

	engine::ref<engine::coin> emitter_shape = engine::coin::create();
	engine::game_object_properties emitter_props;
	emitter_props.meshes = { emitter_shape->mesh() };
	emitter_props.type = 1;
	emitter_props.bounding_shape = glm::vec3(0.5f);
	m_light_emitter = engine::game_object::create(emitter_props);
};

void pickup_speed::on_update(glm::vec3 c, player& player, float dt, engine::ref<engine::audio_manager> m_audio_manager)
{
	if (m_pickup->update(c, dt))
	{
		if (m_audio_manager)
		{
			m_audio_manager->play("pickup");
		}
		player.power_up();
	}
};

void pickup_speed::on_render()
{
	auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
	if (m_pickup->active())
	{
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->
			set_uniform("gNumSpotLights", (int)num_spot_lights);
		m_spot_light.submit(mesh_shader, 0);


		engine::ref<engine::material> m_lightsource_material = engine::material::create(.7f, m_spot_light.Color,
			m_spot_light.Color, m_spot_light.Color, 0.2f);

		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->
			set_uniform("lighting_on", false);
		m_lightsource_material->submit(mesh_shader);
		glm::mat4 emitter_transform(1.0f);
		emitter_transform = glm::translate(emitter_transform, m_spot_light.Position);
		emitter_transform = glm::rotate(emitter_transform, engine::PI / 2, glm::vec3(1.f, 0.f, 0.f));
		emitter_transform = glm::scale(emitter_transform, 6.f * m_pickup->scale());
		engine::renderer::submit(mesh_shader, m_light_emitter->meshes().at(0), emitter_transform);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->
			set_uniform("lighting_on", true);


		/*	engine::ref<engine::material> m_mannequin_material = engine::material::create(1.0f, glm::vec3(1.f, 0.1f, 0.1f),
				glm::vec3(1.f, 0.1f, 0.1f), glm::vec3(1.f, 0.1f, 0.1f), 1.0f);*/
				//m_mannequin_material->submit(mesh_shader);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
		m_pickup->textures().at(0)->bind();
		glm::mat4 pickup_transform(1.0f);
		pickup_transform = glm::translate(pickup_transform, m_position);
		pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
		pickup_transform = glm::scale(pickup_transform, m_pickup->scale());
		engine::renderer::submit(mesh_shader, m_pickup->meshes().at(0), pickup_transform);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
	}
	else
	{
		m_spot_light.Color = glm::vec3(0.f, 0.f, 0.f);
		m_spot_light.submit(mesh_shader, 0);
	}
};

