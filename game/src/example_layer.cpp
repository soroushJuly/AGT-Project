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
	m_audio_manager->load_sound("assets/audio/coin_pick.mp3", engine::sound_type::event, "coin");
	m_audio_manager->load_sound("assets/audio/pickup.wav", engine::sound_type::event, "pickup");
	m_audio_manager->load_sound("assets/audio/run_mud.wav", engine::sound_type::event, "run");
	m_audio_manager->load_sound("assets/audio/walk_mud.wav", engine::sound_type::event, "walk");
	m_audio_manager->load_sound("assets/audio/player_hit.wav", engine::sound_type::event, "hit");
	m_audio_manager->load_sound("assets/audio/take_damage.wav", engine::sound_type::event, "damage");
	m_audio_manager->load_sound("assets/audio/little_village.wav", engine::sound_type::track, "menu");  // Royalty free music from http://www.nosoapradio.us/
	m_audio_manager->load_sound("assets/audio/move_forward.mp3", engine::sound_type::track, "main");  // Royalty free music from http://www.nosoapradio.us/
	m_audio_manager->play("menu");
	//m_audio_manager->pause("music");


	// Initialise the shaders, materials and lights
	auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 0.9f);
	m_directionalLight.AmbientIntensity = 0.38f;
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

	m_mannequin_material = engine::material::create(.8f, glm::vec3(0.1f, 0.9f, 0.9f),
		glm::vec3(0.1f, 0.9f, 0.9f), glm::vec3(0.1f, 0.9f, 0.9f), 1.0f);

	m_cross_fade = cross_fade::create("assets/textures/red.bmp", 2.0f, 1.6f, 0.9f);
	m_billboard = billboard::create("assets/textures/hit.png", 4, 4, 16);
	m_ring.initialise();

	// Free Skybox texture from https://sketchfab.com/3d-models/free-skybox-anime-village-a25aa36a28a14c2e83285ad917947278
	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skybox/skybox_front_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_right_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_back_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_left_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_top_1.png", true),
		  engine::texture_2d::create("assets/textures/skybox/skybox_bottom_1.png", true)
		});

	// Free model from here: https://poly.pizza/m/ZwF0K7WBmu
	engine::ref<engine::skinned_mesh> m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/Adventurer.fbx");
	m_skinned_mesh->switch_root_movement(false);
	m_skinned_mesh->switch_animation(4);

	engine::game_object_properties mannequin_props;
	mannequin_props.animated_mesh = m_skinned_mesh;
	engine::ref<engine::texture_2d> mannequin_texture =
		engine::texture_2d::create("assets/textures/PolyAdventureTexture_01.png", true);
	mannequin_props.textures = { mannequin_texture };
	//mannequin_props.scale = glm::vec3(1.f / glm::max(m_skinned_mesh->size().x, glm::max(m_skinned_mesh->size().y, m_skinned_mesh->size().z)));
	mannequin_props.scale = glm::vec3(0.6f);
	mannequin_props.type = 0;
	mannequin_props.velocity = glm::vec3(0.f);
	mannequin_props.mass = 65.f;
	mannequin_props.position = glm::vec3(0.f, 0.5, 10.f);
	mannequin_props.bounding_shape = glm::vec3(m_skinned_mesh->size().x * mannequin_props.scale.x / 2.f,
		m_skinned_mesh->size().y / mannequin_props.scale.x * 2.4f, m_skinned_mesh->size().x / 2.f);
	m_mannequin = engine::game_object::create(mannequin_props);
	m_player.initialise(m_mannequin);

	m_player_box.set_box(mannequin_props.bounding_shape.x * mannequin_props.scale.x,
		mannequin_props.bounding_shape.y * mannequin_props.scale.x,
		mannequin_props.bounding_shape.z * mannequin_props.scale.x,
		mannequin_props.position);

	// World Collision boxes
	m_world_box_01.set_box(16.f, 10.f, 16.f, mannequin_props.position);
	m_world_box_02.set_box(8.f, 20.f, 70.f, mannequin_props.position + glm::vec3(0.f, -10.f, 40.f));
	m_world_box_03.set_box(36.f, 20.f, 36.f, mannequin_props.position + glm::vec3(14.f, -10.f, 80.f));
	m_world_box_04.set_box(108.f, 20.f, 8.f, mannequin_props.position + glm::vec3(43.f, -10.f, 89.f));
	m_world_box_05.set_box(10.f, 20.f, 70.f, mannequin_props.position + glm::vec3(94.f, -10.f, 50.f));
	m_world_box_06.set_box(28.f, 20.f, 28.f, mannequin_props.position + glm::vec3(84.f, -10.f, 12.f));

	// Free model from here: https://poly.pizza/m/yq5ATpujSt
	engine::ref<engine::skinned_mesh> m_enemy_mesh = engine::skinned_mesh::create("assets/models/animated/Skeleton.fbx");
	m_enemy_mesh->switch_root_movement(false);
	m_enemy_mesh->switch_animation(3);

	engine::game_object_properties skeleton_props;
	std::vector<engine::ref<engine::texture_2d>> tex_vec;
	skeleton_props.animated_mesh = m_enemy_mesh;
	engine::ref<engine::texture_2d> skeleton_texture =
		engine::texture_2d::create("assets/textures/Characters_Brown.png", true);
	skeleton_props.textures = { skeleton_texture };
	skeleton_props.type = 0;
	skeleton_props.mass = 27.2f;
	skeleton_props.velocity = glm::vec3(0.f);
	//skeleton_props.velocity = glm::vec3(10.f);
	skeleton_props.scale = glm::vec3(0.2f);
	skeleton_props.bounding_shape = glm::vec3(m_enemy_mesh->size().x * mannequin_props.scale.x / 2.f,
		m_enemy_mesh->size().y / mannequin_props.scale.x * 2.f, m_enemy_mesh->size().x / 2.f);
	m_skeleton = engine::game_object::create(skeleton_props);

	m_skeleton->set_position(glm::vec3(2.f, 0.5f, 7.f));

	m_enemy_skeleton.initialise(m_skeleton);


	engine::ref<engine::skinned_mesh> m_enemy_mesh_01 = engine::skinned_mesh::create("assets/models/animated/mech.fbx");
	m_enemy_mesh_01->switch_root_movement(false);
	m_enemy_mesh_01->switch_animation(13);

	//engine::ref <engine::model> dd_model = engine::model::create("assets/models/animated/mech.fbx");
	engine::game_object_properties mech_props;
	mech_props.animated_mesh = m_enemy_mesh_01;
	//engine::ref<engine::texture_2d> mech_texture =
	//	engine::texture_2d::create("assets/textures/Atlas.png", false);
	//mech_props.textures = { mech_texture };
	//mech_props.textures = { dd_model->textures() };
	mech_props.type = 0;
	mech_props.position = glm::vec3(0.f, 0.5f, 15.f);
	mech_props.mass = 27.2f;
	mech_props.velocity = glm::vec3(0.f);
	mech_props.scale = glm::vec3(.6f);
	mech_props.bounding_shape = glm::vec3(m_enemy_mesh->size().x * mannequin_props.scale.x / 2.f,
		m_enemy_mesh->size().y * mannequin_props.scale.x / 1.1f, m_enemy_mesh->size().x / 2.f);
	m_mech = engine::game_object::create(mech_props);

	/*m_mech_box.set_box(skeleton_props.bounding_shape.x* skeleton_props.scale.x,
		skeleton_props.bounding_shape.y* skeleton_props.scale.x,
		skeleton_props.bounding_shape.z* skeleton_props.scale.x,
		skeleton_props.position);*/

	m_enemy_mech.initialise(m_mech);


	// initiate spike
	engine::game_object_properties spike_props;
	engine::ref<engine::spike> spike_shape = engine::spike::create(1.f);
	spike_props.meshes = { spike_shape->mesh() };
	spike_props.position = glm::vec3(0.f, .5f, 4.f);
	spike = engine::game_object::create(spike_props);

	// Initialize objects
	m_pickup_heart_01.on_initialize(glm::vec3(0.f, 1.2f, 21.f));
	m_pickup_speed_01.on_initialize(glm::vec3(3.f, 1.2f, 22.f));
	//m_pickup_heart_02.on_initialize(glm::vec3(0.f, 1.2f, 2.f));
	m_pickup_coin_01.on_initialize(glm::vec3(0.f, 1.2f, 20.f));
	m_pickup_coin_02.on_initialize(glm::vec3(0.f, 1.2f, 21.f));
	m_pickup_coin_03.on_initialize(glm::vec3(0.f, 1.4f, 22.f));
	m_pickup_coin_04.on_initialize(glm::vec3(0.f, 1.6f, 23.f));
	m_pickup_coin_05.on_initialize(glm::vec3(0.f, 1.4f, 24.f));
	m_pickup_coin_06.on_initialize(glm::vec3(0.f, 1.2f, 25.f));
	m_pickup_coin_07.on_initialize(glm::vec3(0.f, 1.f, 26.f));

	// Initilaise the decorations in the map.
	m_decorations.on_initialise();

	m_game_intro = game_intro::create("assets/textures/intro_screen.jpg", 1.6f, 0.9f);
	m_game_won = game_intro::create("assets/textures/game_won.png", 1.6f, 0.9f);
	m_game_lost = game_intro::create("assets/textures/game_lost.png", 1.6f, 0.9f);
	hud.on_initialize();

	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	// Texture from POLYGON Pack https://syntystore.com/products/polygon-adventure-pack?_pos=1&_psq=adve&_ss=e&_v=1.0
	m_heightmap = engine::heightmap::create("assets/textures/ht_01.jpg", "assets/textures/grass.png", 180.f, 180.f, glm::vec3(48.f, 0.5f, 40.f), 2.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { m_heightmap->mesh() };
	terrain_props.textures = { m_heightmap->texture() };
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(m_heightmap->terrain_size().x, m_physical_terrain_height, m_heightmap->terrain_size().y);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/mud.png", false) };
	engine::ref<engine::terrain> terrain_shape_2 = engine::terrain::create(200.f, 0.5f, 200.f, 100);
	engine::game_object_properties terrain_props_2;
	terrain_props_2.meshes = { terrain_shape_2->mesh() };
	terrain_props_2.textures = terrain_textures;
	terrain_props_2.is_static = true;
	terrain_props_2.type = 0;
	terrain_props_2.bounding_shape = glm::vec3(100.f, 0.5f, 100.f);
	terrain_props_2.restitution = 0.92f;
	m_terrain_2 = engine::game_object::create(terrain_props_2);

	// TODO: jumping pads on lava
	m_lava_01.on_initialise(glm::vec3(0.f, 0.5f, 30.f), glm::vec3(4.5f, .01f, 1.5f));
	m_lava_02.on_initialise(glm::vec3(0.f, 0.5f, 60.f), glm::vec3(4.5f, .01f, 1.5f));
	m_lava_03.on_initialise(glm::vec3(35.f, 0.5f, 98.f), glm::vec3(1.5f, .01f, 4.5f));
	m_lava_04.on_initialise(glm::vec3(55.f, 0.5f, 98.f), glm::vec3(1.5f, .01f, 4.5f));
	m_lava_05.on_initialise(glm::vec3(93.6f, 0.5f, 55.f), glm::vec3(4.5f, .01f, 1.5f));
	m_lava_06.on_initialise(glm::vec3(93.6f, 0.5f, 70.f), glm::vec3(4.5f, .01f, 1.5f));


	m_game_objects.push_back(m_terrain);
	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step)
{
	if (m_state == example_layer::MENU)
		return;
	if (m_player.is_dead())
	{
		m_state = example_layer::GAME_LOST;
		return;
	}
	if (m_enemy_mech.is_dead())
		m_state = example_layer::GAME_WON;

	// Uncomment to roam around the map
	//m_3d_camera.on_update(time_step);

	m_player_box.set_box(m_player.object()->bounding_shape().x * m_player.object()->scale().x,
		m_player.object()->bounding_shape().y * m_player.object()->scale().x,
		m_player.object()->bounding_shape().z * m_player.object()->scale().x,
		m_player.position());

	m_pickup_heart_01.on_update(m_player.position(), m_player.hearts(), time_step, m_audio_manager);
	m_pickup_speed_01.on_update(m_player.position(), m_player.speed(), time_step, m_audio_manager);
	//m_pickup_heart_02.on_update(m_player.position(), m_player.hearts(), time_step, m_audio_manager);
	m_pickup_coin_01.on_update(m_player.position(), m_player.coins(), time_step, m_audio_manager);
	m_pickup_coin_02.on_update(m_player.position(), m_player.coins(), time_step, m_audio_manager);
	m_pickup_coin_03.on_update(m_player.position(), m_player.coins(), time_step, m_audio_manager);
	m_pickup_coin_04.on_update(m_player.position(), m_player.coins(), time_step, m_audio_manager);
	m_pickup_coin_05.on_update(m_player.position(), m_player.coins(), time_step, m_audio_manager);
	m_pickup_coin_06.on_update(m_player.position(), m_player.coins(), time_step, m_audio_manager);
	m_pickup_coin_07.on_update(m_player.position(), m_player.coins(), time_step, m_audio_manager);

	m_enemy_mech.on_update(time_step, m_player.position());

	m_cross_fade->on_update(time_step);
	m_billboard->on_update(time_step);

	m_ring.on_update(time_step, m_player.position());

	m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

	glm::vec3 pos = m_player.object()->position();
	m_player.on_update(time_step);
	m_player.update_camera(m_3d_camera, time_step);
	m_player_box.on_update(m_player.object()->position());

	m_enemy_skeleton.on_update(time_step, m_player_box, m_player.object()->position(), m_player.is_punching());

	if (m_player.is_punching())
	{
		float x_position = m_player.position().x + m_player.object()->bounding_shape().x / 2;
		float y_position = m_player.position().y + m_player.object()->bounding_shape().y / 2;
		float z_position = m_player.position().z - m_player.object()->bounding_shape().z / 2;
		m_billboard->activate(glm::vec3(x_position, y_position, z_position), 2.f, 2.f);
	}
	// Player move restriction in the map
	if (!(m_world_box_01.collision(m_player_box)
		|| m_world_box_02.collision(m_player_box)
		|| m_world_box_03.collision(m_player_box)
		|| m_world_box_04.collision(m_player_box)
		|| m_world_box_05.collision(m_player_box)
		|| m_world_box_06.collision(m_player_box)
		))
	{
		m_player.object()->set_position(pos);
	}
	if (m_lava_01.collision(m_player_box)
		|| m_lava_02.collision(m_player_box)
		|| m_lava_03.collision(m_player_box)
		|| m_lava_04.collision(m_player_box)
		|| m_lava_05.collision(m_player_box)
		|| m_lava_06.collision(m_player_box)
		)
	{
		m_player.take_damage(m_audio_manager, m_cross_fade, time_step);
	}

	hud.on_update(time_step, m_player.hearts());
	m_skeleton->animated_mesh()->on_update(time_step);


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

	// Render 2D Camera
	engine::renderer::begin_scene(m_2d_camera, mesh_shader);
	if (m_state == example_layer::MENU)
		m_game_intro->on_render(mesh_shader);
	else if (m_state == example_layer::GAME_WON)
		m_game_won->on_render(mesh_shader);
	else if (m_state == example_layer::GAME_LOST)
		m_game_lost->on_render(mesh_shader);
	engine::renderer::end_scene();

	engine::renderer::begin_scene(m_3d_camera, mesh_shader);

	// Set up some of the scene's parameters in the shader
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	m_player_box.on_render(2.5f, 1.f, 1.f, mesh_shader);
	
	//m_lava_box.on_render(2.5f, 1.f, 1.f, mesh_shader);
	m_world_box_01.on_render(2.5f, 1.f, 1.f, mesh_shader);
	m_world_box_03.on_render(2.5f, 1.f, 1.f, mesh_shader);
	m_world_box_04.on_render(2.5f, 1.f, 1.f, mesh_shader);
	m_world_box_05.on_render(2.5f, 1.f, 1.f, mesh_shader);
	m_world_box_06.on_render(2.5f, 1.f, 1.f, mesh_shader);





	// Position the skybox centred on the player and render it
	glm::mat4 skybox_tranform(1.0f);
	skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
	for (const auto& texture : m_skybox->textures())
	{
		texture->bind();
	}
	engine::renderer::submit(mesh_shader, m_skybox, skybox_tranform);

	engine::renderer::submit(mesh_shader, m_terrain);
	engine::renderer::submit(mesh_shader, m_terrain_2);

	m_lava_01.on_render(mesh_shader);
	m_lava_02.on_render(mesh_shader);
	m_lava_03.on_render(mesh_shader);
	m_lava_04.on_render(mesh_shader);
	m_lava_05.on_render(mesh_shader);
	m_lava_06.on_render(mesh_shader);

	// Render Objects in the scene
	m_pickup_speed_01.on_render();
	m_pickup_heart_01.on_render();
	//m_pickup_heart_02.on_render();
	m_pickup_coin_01.on_render();
	m_pickup_coin_02.on_render();
	m_pickup_coin_03.on_render();
	m_pickup_coin_04.on_render();
	m_pickup_coin_05.on_render();
	m_pickup_coin_06.on_render();
	m_pickup_coin_07.on_render();

	// Render all of the decorations in the map.
	m_decorations.on_render(mesh_shader);


	m_mannequin_material->submit(mesh_shader);
	engine::renderer::submit(mesh_shader, spike);
	engine::renderer::submit(mesh_shader, m_player.object());



	m_enemy_skeleton.on_render(mesh_shader, m_3d_camera);
	m_enemy_mech.on_render(mesh_shader);

	m_ring.on_render(mesh_shader);

	engine::renderer::end_scene();

	engine::renderer::begin_scene(m_2d_camera, mesh_shader);
	hud.on_render(mesh_shader);
	engine::renderer::end_scene();

	engine::renderer::begin_scene(m_2d_camera, mesh_shader);
	m_cross_fade->on_render(mesh_shader);
	engine::renderer::end_scene();

	engine::renderer::begin_scene(m_3d_camera, mesh_shader);
	//m_billboard->on_render(m_3d_camera, mesh_shader);
	engine::renderer::end_scene();
	// Render text
	m_text_manager->render_text(text_shader, std::to_string(m_player.coins()), 43.f, (float)engine::application::window().height() - 91.f, 0.5f, glm::vec4(1.f, 0.85f, 0.f, 1.f));
	m_text_manager->render_text(text_shader, std::to_string(m_play_time.total()), 45.f, (float)engine::application::window().height() - 135.f, 0.5f, glm::vec4(.36f, 0.25f, 0.2f, 1.f));
}

void example_layer::on_event(engine::event& event)
{
	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);
		if (m_state == example_layer::MENU && e.key_code() == engine::key_codes::KEY_ENTER)
		{
			m_state = example_layer::IN_GAME;
			m_audio_manager->stop("menu");
			m_audio_manager->play("music");
			m_play_time.start();
		}
		if ((m_state == example_layer::GAME_LOST || m_state == example_layer::GAME_WON) && e.key_code() == engine::key_codes::KEY_ESCAPE)
		{
			engine::application::exit();
		}

		if (e.key_code() == engine::key_codes::KEY_TAB)
		{
			engine::render_command::toggle_wireframe();
		}
		if (e.key_code() == engine::key_codes::KEY_1)
		{
			m_ring.activate(.3f, m_player.position());
		}
		if (e.key_code() == engine::key_codes::KEY_2)
		{
			m_enemy_skeleton.take_damage();
		}
		if (e.key_code() == engine::key_codes::KEY_3)
		{
			m_enemy_mech.shoot_bomb(m_player.position());
		}
		if (e.key_code() == engine::key_codes::KEY_4)
		{
			m_enemy_mech.shoot_rocket();
		}
	}
}
