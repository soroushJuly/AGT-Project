#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include "pickup.h"
#include "static_object.h"

example_layer::example_layer()
	:m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f),
	m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())


{
	// Hide the mouse and lock it inside the window
	//engine::input::anchor_mouse(true);
	engine::application::window().hide_mouse_cursor();

	// Initialise audio and play background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	m_audio_manager->load_sound("assets/audio/bounce.wav", engine::sound_type::spatialised, "bounce"); // Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/DST-impuretechnology.mp3", engine::sound_type::track, "music");  // Royalty free music from http://www.nosoapradio.us/
	m_audio_manager->play("music");
	m_audio_manager->pause("music");


	// Initialise the shaders, materials and lights
	auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.25f;
	m_directionalLight.DiffuseIntensity = 0.6f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

	// set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", true);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gColorMap", 0);
	m_directionalLight.submit(mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gMatSpecularIntensity", 1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gSpecularPower", 10.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("transparency", 1.0f);

	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
			(float)engine::application::window().height()));
	m_material = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	m_tetrahedron_material = engine::material::create(32.0f, glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 0.5f);

	m_mannequin_material = engine::material::create(1.0f, glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);


	// Skybox texture from http://www.vwall.it/wp-content/plugins/canvasio3dpro/inc/resource/cubeMaps/
	//m_skybox = engine::skybox::create(50.f,
	//	{ engine::texture_2d::create("assets/textures/skybox/SkyboxFront.bmp", true),
	//	  engine::texture_2d::create("assets/textures/skybox/SkyboxRight.bmp", true),
	//	  engine::texture_2d::create("assets/textures/skybox/SkyboxBack.bmp", true),
	//	  engine::texture_2d::create("assets/textures/skybox/SkyboxLeft.bmp", true),
	//	  engine::texture_2d::create("assets/textures/skybox/SkyboxTop.bmp", true),
	//	  engine::texture_2d::create("assets/textures/skybox/SkyboxBottom.bmp", true)
	//	});
	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skybox/skybox_front_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_right_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_back_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_left_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_top_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_bottom_1.png", true)
		});

	engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
	m_skinned_mesh->switch_root_movement(false);

	engine::game_object_properties mannequin_props;
	mannequin_props.animated_mesh = m_skinned_mesh;
	mannequin_props.scale = glm::vec3(1.f / glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	mannequin_props.position = glm::vec3(.0f, .5f, -5.0f);
	mannequin_props.type = 0;
	mannequin_props.bounding_shape = m_skinned_mesh->size() / 2.f * mannequin_props.scale.x;
	m_mannequin = engine::game_object::create(mannequin_props);
	m_player.initialise(m_mannequin);

	// creating the pickup object
	engine::ref<engine::cuboid> pickup_shape = engine::cuboid::create(glm::vec3(0.5f), false);
	engine::ref<engine::texture_2d> pickup_texture =
		engine::texture_2d::create("assets/textures/medkit.jpg", true);
	engine::game_object_properties pickup_props;
	pickup_props.position = { 5.f, 1.f, 5.f };
	pickup_props.meshes = { pickup_shape->mesh() };
	pickup_props.textures = { pickup_texture };
	m_pickup = pickup::create(pickup_props);
	m_pickup->init();

	// creating another pickup object
	m_pickup_coin.on_initialize();
	bush.on_initialize("assets/models/static/SM_Env_Bush_01.fbx", "assets/textures/grass.png", glm::vec3(4.f, .45f, -3.5f));
	tree_01.on_initialize("assets/models/static/SM_Env_Tree_02.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(4.f, 0, -2.f));
	tree_02.on_initialize("assets/models/static/SM_Env_Tree_04.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(-4.f, 0, -2.f));
	tree_03.on_initialize("assets/models/static/SM_Env_Tree_08.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(-4.f, 0, -4.f));
	//fence.on_initialize("assets/models/FBX/SM_Plant_01.fbx", "assets/textures/grass.png");

	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/mud.png", false) };
	engine::ref<engine::terrain> terrain_shape = engine::terrain::create(100.f, 0.5f, 100.f, 70);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(100.f, 0.5f, 100.f);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(10, 20, 0.5f);
	engine::game_object_properties sphere_props;
	sphere_props.position = { 0.f, 5.f, -5.f };
	sphere_props.meshes = { sphere_shape->mesh() };
	sphere_props.type = 1;
	sphere_props.bounding_shape = glm::vec3(0.5f);
	sphere_props.restitution = 0.92f;
	sphere_props.mass = 0.000001f;
	m_ball = engine::game_object::create(sphere_props);

	//std::vector<glm::vec3> tetrahedron_vertices;
	//tetrahedron_vertices.push_back(glm::vec3(0.f, 10.f, 0.f)); //0
	//tetrahedron_vertices.push_back(glm::vec3(0.f, 0.f, 10.f)); //1
	//tetrahedron_vertices.push_back(glm::vec3(-10.f, 0.f, -10.f)); //2
	//tetrahedron_vertices.push_back(glm::vec3(10.f, 0.f, -10.f)); //3
	//engine::ref<engine::tetrahedron> tetrahedron_shape = engine::tetrahedron::create(tetrahedron_vertices);

	m_game_objects.push_back(m_terrain);
	m_game_objects.push_back(m_ball);
	//m_game_objects.push_back(m_cow);
	//m_game_objects.push_back(m_tree);
	//m_game_objects.push_back(m_pickup);
	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();

	m_skinned_mesh->switch_animation(1);
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step)
{
	m_3d_camera.on_update(time_step);

	m_pickup->update(m_3d_camera.position(), time_step);
	m_pickup_coin.on_update(m_3d_camera.position(), time_step);
	/*m_pickup_2->update(m_3d_camera.position(), time_step);*/

	m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

	m_player.on_update(time_step);
	//m_player.update_camera(m_3d_camera, time_step);

	m_audio_manager->update_with_camera(m_3d_camera);

	check_bounce();
}

void example_layer::on_render()
{
	engine::render_command::clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
	engine::render_command::clear();

	// Set up  shader. (renders textures and materials)
	const auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
	engine::renderer::begin_scene(m_3d_camera, mesh_shader);

	// Set up some of the scene's parameters in the shader
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	// Position the skybox centred on the player and render it
	glm::mat4 skybox_tranform(1.0f);
	skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
	for (const auto& texture : m_skybox->textures())
	{
		texture->bind();
	}
	engine::renderer::submit(mesh_shader, m_skybox, skybox_tranform);

	engine::renderer::submit(mesh_shader, m_terrain);


	if (m_pickup->active())
	{
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
		m_pickup->textures().at(0)->bind();
		glm::mat4 pickup_transform(1.0f);
		pickup_transform = glm::translate(pickup_transform, m_pickup->position());
		pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
		engine::renderer::submit(mesh_shader, m_pickup->meshes().at(0), pickup_transform);
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
	}

	// pickup_coin rendering
	m_pickup_coin.on_render();
	bush.on_render(mesh_shader);
	// rotated bush
	bush.on_render(mesh_shader, glm::vec3(4.f, .45f, -6.25f), engine::PI / 2, glm::vec3(0.f, 1.f, 0.f), glm::vec3(.013f, .01f, .013f));
	tree_01.on_render(mesh_shader);
	tree_02.on_render(mesh_shader);
	tree_03.on_render(mesh_shader);
	tree_03.on_render(mesh_shader);
	tree_03.on_render(mesh_shader, glm::vec3(-4.f, 0, -7.f), 0.f, glm::vec3(0.f, 1.f, 0.f), glm::vec3(.013f, .018f, .013f));
	tree_03.on_render(mesh_shader, glm::vec3(-4.3f, 0, -10.3f), engine::PI, glm::vec3(0.f, 1.f, 0.f), glm::vec3(.013f));

	m_material->submit(mesh_shader);
	engine::renderer::submit(mesh_shader, m_ball);

	//m_tetrahedron_material->submit(mesh_shader);
	//engine::renderer::submit(mesh_shader, m_tetrahedron);

	//coin_transform = glm::rotate(coin_transform, engine::PI * 22.5f / 180, glm::vec3(0.f, 0.f, -1.f));

	m_mannequin_material->submit(mesh_shader);
	engine::renderer::submit(mesh_shader, m_player.object());

	engine::renderer::end_scene();

	// Render text
	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	m_text_manager->render_text(text_shader, "Orange Text", 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 0.5f, 0.f, 1.f));
}

void example_layer::on_event(engine::event& event)
{
	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);
		if (e.key_code() == engine::key_codes::KEY_TAB)
		{
			engine::render_command::toggle_wireframe();
		}
	}
}

void example_layer::check_bounce()
{
	if (m_prev_sphere_y_vel < 0.1f && m_ball->velocity().y > 0.1f)
		//m_audio_manager->play("bounce");
		m_audio_manager->play_spatialised_sound("bounce", m_3d_camera.position(), glm::vec3(m_ball->position().x, 0.f, m_ball->position().z));
	m_prev_sphere_y_vel = m_game_objects.at(1)->velocity().y;
}
