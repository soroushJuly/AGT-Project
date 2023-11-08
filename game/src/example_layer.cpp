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
	m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height()),
	is_intro_active(true)
{
	// Hide the mouse and lock it inside the window
	//engine::input::anchor_mouse(true);
	engine::application::window().hide_mouse_cursor();

	// Initialise audio and play background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
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

	m_mannequin_material = engine::material::create(1.0f, glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);


	// Free Skybox texture from https://sketchfab.com/3d-models/free-skybox-anime-village-a25aa36a28a14c2e83285ad917947278
	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skybox/skybox_front_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_right_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_back_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_left_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_top_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_bottom_1.png", true)
		});

	engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/Adventurer.fbx");
	//engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/Characters_Skeleton.fbx");
	//m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	//m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
	//m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
	//m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");
	m_skinned_mesh->set_default_animation(3);
	m_skinned_mesh->switch_root_movement(false);

	engine::game_object_properties mannequin_props;
	mannequin_props.animated_mesh = m_skinned_mesh;
	engine::ref<engine::texture_2d> mannequin_texture =
		engine::texture_2d::create("assets/textures/PolyAdventureTexture_01.png", true);
	mannequin_props.textures = { mannequin_texture };
	mannequin_props.scale = glm::vec3(1.f / glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	mannequin_props.position = glm::vec3(.0f, .5f, -5.0f);
	mannequin_props.type = 0;
	mannequin_props.bounding_shape = m_skinned_mesh->size() / 2.f * mannequin_props.scale.x;
	m_mannequin = engine::game_object::create(mannequin_props);
	m_player.initialise(m_mannequin);

	// Initialize objects
	m_pickup_coin_01.on_initialize();
	m_pickup_coin_02.on_initialize(glm::vec3(0.f, 1.2f, -1.f));
	m_pickup_coin_03.on_initialize(glm::vec3(0.f, 1.4f, -2.f));
	m_pickup_coin_04.on_initialize(glm::vec3(0.f, 1.6f, -3.f));
	m_pickup_coin_05.on_initialize(glm::vec3(0.f, 1.4f, -4.f));
	m_pickup_coin_06.on_initialize(glm::vec3(0.f, 1.2f, -5.f));
	m_pickup_coin_07.on_initialize(glm::vec3(0.f, 1.f, -6.f));
	// Textures below from POLYGON Pack https://syntystore.com/products/polygon-adventure-pack?_pos=1&_psq=adve&_ss=e&_v=1.0
	bush.on_initialize("assets/models/static/SM_Env_Bush_01.fbx", "assets/textures/grass.png", glm::vec3(4.f, .45f, -3.5f));
	tree_01.on_initialize("assets/models/static/SM_Env_Tree_02.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(4.f, 0, -2.f));
	tree_02.on_initialize("assets/models/static/SM_Env_Tree_04.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(-4.f, 0, -2.f));
	tree_03.on_initialize("assets/models/static/SM_Env_Tree_08.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(-4.f, 0, -4.f));

	m_game_intro = game_intro::create("assets/textures/intro_screen.jpg", 1.6f, 0.9f);

	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	// Texture from POLYGON Pack https://syntystore.com/products/polygon-adventure-pack?_pos=1&_psq=adve&_ss=e&_v=1.0
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


	m_game_objects.push_back(m_terrain);
	//m_game_objects.push_back(m_cow);
	//m_game_objects.push_back(m_pickup);
	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step)
{
	if (is_intro_active)
	{
		return;
	}
	//m_3d_camera.on_update(time_step);

	m_pickup_coin_01.on_update(m_player.position(), m_player.coins(), time_step);
	m_pickup_coin_02.on_update(m_player.position(), m_player.coins(), time_step);
	m_pickup_coin_03.on_update(m_player.position(), m_player.coins(), time_step);
	m_pickup_coin_04.on_update(m_player.position(), m_player.coins(), time_step);
	m_pickup_coin_05.on_update(m_player.position(), m_player.coins(), time_step);
	m_pickup_coin_06.on_update(m_player.position(), m_player.coins(), time_step);
	m_pickup_coin_07.on_update(m_player.position(), m_player.coins(), time_step);

	m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

	m_player.on_update(time_step);
	m_player.update_camera(m_3d_camera, time_step);

	m_audio_manager->update_with_camera(m_3d_camera);
}

void example_layer::on_render()
{
	engine::render_command::clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
	engine::render_command::clear();
	// Set up text shader
	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	// Set up  shader. (renders textures and materials)
	const auto mesh_shader = engine::renderer::shaders_library()->get("mesh");

	// render 2D Camera
	engine::renderer::begin_scene(m_2d_camera, mesh_shader);
	m_game_intro->on_render(mesh_shader);
	engine::renderer::end_scene();

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

	// Render Objects in the scene
	m_pickup_coin_01.on_render();
	m_pickup_coin_02.on_render();
	m_pickup_coin_03.on_render();
	m_pickup_coin_04.on_render();
	m_pickup_coin_05.on_render();
	m_pickup_coin_06.on_render();
	m_pickup_coin_07.on_render();
	bush.on_render(mesh_shader);
	bush.on_render(mesh_shader, glm::vec3(4.f, .45f, -6.25f), engine::PI / 2, glm::vec3(0.f, 1.f, 0.f), glm::vec3(.013f, .01f, .013f));
	tree_01.on_render(mesh_shader);
	tree_02.on_render(mesh_shader);
	tree_03.on_render(mesh_shader);
	tree_03.on_render(mesh_shader);
	tree_03.on_render(mesh_shader, glm::vec3(-4.f, 0, -7.f), 0.f, glm::vec3(0.f, 1.f, 0.f), glm::vec3(.013f, .018f, .013f));
	tree_03.on_render(mesh_shader, glm::vec3(-4.3f, 0, -10.3f), engine::PI, glm::vec3(0.f, 1.f, 0.f), glm::vec3(.013f));

	m_mannequin_material->submit(mesh_shader);
	engine::renderer::submit(mesh_shader, m_player.object());

	engine::renderer::end_scene();

	// Render text
	m_text_manager->render_text(text_shader, "Coins: " + std::to_string(m_player.coins()), 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 0.85f, 0.f, 1.f));
	m_text_manager->render_text(text_shader, "Health: 100", 10.f, (float)engine::application::window().height() - 50.f, 0.5f, glm::vec4(1.f, 0.1f, 0.1f, 1.f));
	m_text_manager->render_text(text_shader, "Time: 0s", 10.f, (float)engine::application::window().height() - 75.f, 0.5f, glm::vec4(.36f, 0.25f, 0.2f, 1.f));
}

void example_layer::on_event(engine::event& event)
{
	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);
		if (is_intro_active && e.key_code() == engine::key_codes::KEY_ENTER)
		{
			is_intro_active = false;
			m_game_intro->deactivate();
		}

		if (e.key_code() == engine::key_codes::KEY_TAB)
		{
			engine::render_command::toggle_wireframe();
		}
	}
}
