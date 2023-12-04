#include "level.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include "pickup.h"
#include "static_object.h"

level::level()
	:m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f),
	m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height(),45.f,0.1f,500.f),
	m_reached_time(0.f)
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
	m_audio_manager->volume("menu", 0.07f);
	//m_audio_manager->pause("music");


	// Initialise the shaders, materials and lights
	auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 0.85f);
	m_directionalLight.AmbientIntensity = 0.6f;
	m_directionalLight.DiffuseIntensity = 0.6f;
	const float theta = engine::PI / 6;
	m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f * cos(theta), -1.0f * sin(theta), 1.0f * cos(theta) * 0.2));

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

	m_mannequin_material = engine::material::create(.8f, glm::vec3(0.1f, 0.3f, 0.3f),
		glm::vec3(0.1f, 0.3f, 0.3f), glm::vec3(0.1f, 0.3f, 0.3f), 1.0f);

	m_cross_fade = cross_fade::create("assets/textures/red.bmp", 2.0f, 1.6f, 0.9f);
	m_ring.initialise();

	// Free Skybox texture from https://sketchfab.com/3d-models/free-skybox-anime-village-a25aa36a28a14c2e83285ad917947278
	m_skybox = engine::skybox::create(250.f,
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
	mannequin_props.scale = glm::vec3(0.6f);
	mannequin_props.type = 0;
	mannequin_props.velocity = glm::vec3(0.f);
	mannequin_props.mass = 65.f;
	mannequin_props.position = glm::vec3(0.f, 0.5, 10.f);
	mannequin_props.bounding_shape = glm::vec3(m_skinned_mesh->size().x * mannequin_props.scale.x / 2.f,
		m_skinned_mesh->size().y / mannequin_props.scale.x * 2.4f, m_skinned_mesh->size().x / 2.f);
	m_mannequin = engine::game_object::create(mannequin_props);
	m_player.initialise(m_mannequin, m_cross_fade, m_audio_manager);

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


	// TODO: add texture for the mech (using model create? then ->textures)
	engine::ref<engine::skinned_mesh> m_enemy_mesh_01 = engine::skinned_mesh::create("assets/models/animated/mech.fbx");
	m_enemy_mesh_01->switch_root_movement(false);
	m_enemy_mesh_01->switch_animation(13);

	engine::game_object_properties mech_props;
	mech_props.animated_mesh = m_enemy_mesh_01;
	mech_props.position = glm::vec3(85.f, 0.5f, 19.f);
	mech_props.velocity = glm::vec3(0.f);
	mech_props.scale = glm::vec3(.6f);
	mech_props.bounding_shape = glm::vec3(m_enemy_mesh_01->size().x * mannequin_props.scale.x / 2.f,
		m_enemy_mesh_01->size().y * mannequin_props.scale.x / 1.1f, m_enemy_mesh_01->size().x / 2.f);
	m_mech = engine::game_object::create(mech_props);

	m_enemy_mech.initialise(m_mech);

	// Add skeletons to the map
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m_skeleton_list.push_back(enemy_basic_skeleton::create(glm::vec3(3.f + 3.f * (float)i, .5f, 88.f + 3.f * (float)j)));
		}
	}

	// TODO: these numbers should be constant floats like: MIDDLE_POINT_BOX_01
	m_crab_list.push_back(enemy_basic_crab::create(glm::vec3(0.f, .5f, 50.f)));
	m_crab_list.push_back(enemy_basic_crab::create(glm::vec3(0.f, .5f, 76.f)));
	m_crab_list.push_back(enemy_basic_crab::create(glm::vec3(37.f, .5f, 98.f)));
	m_crab_list.push_back(enemy_basic_crab::create(glm::vec3(62.f, .5f, 97.f)));
	m_crab_list.push_back(enemy_basic_crab::create(glm::vec3(93.6f, .5f, 45.f)));
	m_crab_list.push_back(enemy_basic_crab::create(glm::vec3(93.6f, .5f, 80.f)));

	// Add robots to the map
	for (size_t i = 0; i < 4; i++)
		for (size_t j = 0; j < 3; j++)
			m_robot_list.push_back(enemy_basic_robot::create(glm::vec3(91.f + 3.f * (float)i, .5f, 88.f + 3.f * (float)j)));

	// Add spikes to the map
	m_spike_list.push_back(enemy_spike::create(glm::vec3(0.f, 0.5f, 40.f), 4.f, 0.f));
	m_spike_list.push_back(enemy_spike::create(glm::vec3(0.f, 0.5f, 67.f), 4.f, 0.f));
	m_spike_list.push_back(enemy_spike::create(glm::vec3(42.f, 0.5f, 98.5f), 3.5f, engine::PI / 2));
	m_spike_list.push_back(enemy_spike::create(glm::vec3(67.f, 0.5f, 98.5f), 3.5f, engine::PI / 2));
	m_spike_list.push_back(enemy_spike::create(glm::vec3(93.6f, 0.5f, 40.f), 3.8f, 0.f));
	m_spike_list.push_back(enemy_spike::create(glm::vec3(93.6f, 0.5f, 60.f), 3.8f, 0.f));

	// TODO: jumping pads on lava
	// TODO: add a list for lava like other enemies (makes the rendering easier)
	m_lava_01.on_initialise(glm::vec3(0.f, 0.5f, 30.f), glm::vec3(4.5f, .01f, 1.5f));
	m_lava_02.on_initialise(glm::vec3(0.f, 0.5f, 60.f), glm::vec3(4.5f, .01f, 1.5f));
	m_lava_03.on_initialise(glm::vec3(35.f, 0.5f, 98.f), glm::vec3(1.5f, .01f, 4.5f));
	m_lava_04.on_initialise(glm::vec3(55.f, 0.5f, 98.f), glm::vec3(1.5f, .01f, 4.5f));
	m_lava_05.on_initialise(glm::vec3(93.6f, 0.5f, 50.f), glm::vec3(4.5f, .01f, 1.5f));
	m_lava_06.on_initialise(glm::vec3(93.6f, 0.5f, 70.f), glm::vec3(4.5f, .01f, 1.5f));

	// Add coins in the map
	for (size_t i = 0; i < 10; i++)
	{
		m_coin_list.push_back(pickup_coin::create(glm::vec3(0.f, 1.f, 20.f + (float)i / 2.f)));
		m_coin_list.push_back(pickup_coin::create(glm::vec3(-2.5f, 1.f, 35.f + (float)i / 2.f)));
		m_coin_list.push_back(pickup_coin::create(glm::vec3(2.5f, 1.f, 35.f + (float)i / 2.f)));
		m_coin_list.push_back(pickup_coin::create(glm::vec3(-2.1f, 1.f, 47.f + (float)i / 2.f)));
		m_coin_list.push_back(pickup_coin::create(glm::vec3(2.1f, 1.f, 47.f + (float)i / 2.f)));
		m_coin_list.push_back(pickup_coin::create(glm::vec3(8.f + (float)i / 2.f, 1.4f, 83.f + (float)i / 2.f)));

		m_coin_list.push_back(pickup_coin::create(glm::vec3(16.f + (float)i / 2.f, 1.f, 98.f)));
		m_coin_list.push_back(pickup_coin::create(glm::vec3(60.f + (float)i / 2.f, 1.f, 97.f)));
		m_coin_list.push_back(pickup_coin::create(glm::vec3(60.f + (float)i / 2.f, 1.f, 100.f)));
		m_coin_list.push_back(pickup_coin::create(glm::vec3(90.f + (float)i / 2.f, 1.f, 80.f - (float)i / 2.f)));
		m_coin_list.push_back(pickup_coin::create(glm::vec3(90.f + (float)i / 2.f, 1.f, 68.f - (float)i / 2.f)));
	}

	// Initialize heart and speed-up
	// TODO: these will be drop-downs from enemies
	m_pickup_heart_01.on_initialize(glm::vec3(93.6f, 1.2f, 94.f));
	m_pickup_speed_01.on_initialize(glm::vec3(10.f, 1.2f, 90.f));
	//m_pickup_heart_02.on_initialize(glm::vec3(0.f, 1.2f, 2.f));

	// Initialise all the decorations in the map.
	m_decorations.on_initialise();

	// Initialise 2D features
	m_game_intro = game_intro::create("assets/textures/Intro_screen_01.jpg", 1.6f, 0.9f);
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

	m_game_objects.push_back(m_terrain_2);
	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();
}

level::~level() {}

void level::on_update(const engine::timestep& time_step)
{
	if (m_state == level::MENU)
		return;
	if (m_player.is_dead())
	{
		//m_audio_manager->stop("main");
		m_state = level::GAME_LOST;
		return;
	}
	if (m_enemy_mech.is_dead())
	{
		m_audio_manager->stop("main");
		m_state = level::GAME_WON;
	}

	const float TIME_LIMIT = 160.f;
	m_remained_time = TIME_LIMIT - (float)m_play_time.total();
	if (m_reached_time == 0.f && m_remained_time < 0.f)
		m_state = level::GAME_LOST;

	// Uncomment to roam around the map
	//m_3d_camera.on_update(time_step);

	m_player_box.set_box(m_player.object()->bounding_shape().x * m_player.object()->scale().x,
		m_player.object()->bounding_shape().y * m_player.object()->scale().x,
		m_player.object()->bounding_shape().z * m_player.object()->scale().x,
		m_player.position());

	m_pickup_heart_01.on_update(m_player.position(), m_player, time_step, m_audio_manager);
	m_pickup_speed_01.on_update(m_player.position(), m_player, time_step, m_audio_manager);
	for (auto coin : m_coin_list)
	{
		coin->on_update(m_player.position(), m_player, m_player_box, time_step, m_audio_manager);
	}

	m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

	glm::vec3 pos = m_player.object()->position();
	m_player.on_update(time_step);
	m_player.update_camera(m_3d_camera, time_step);
	m_player_box.on_update(m_player.object()->position());

	m_enemy_mech.on_update(time_step, m_player, m_player_box, m_player.position());
	// TODO: Not passing the player - instead pointer
	for (auto enemy : m_skeleton_list)
	{
		enemy->on_update(time_step, m_player, m_player_box);
	}
	for (auto enemy : m_robot_list)
	{
		enemy->on_update(time_step, m_player, m_player_box);
	}
	for (auto enemy : m_crab_list)
		enemy->on_update(time_step, m_player, m_player_box);

	for (auto enemy : m_spike_list)
	{
		enemy->on_update(time_step, m_player, m_player_box);
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

	// Stop the timer when player reaches the boss
	if (m_reached_time == 0.f && m_world_box_06.collision(m_player_box))
		m_reached_time = m_remained_time;

	if (m_lava_01.collision(m_player_box)
		|| m_lava_02.collision(m_player_box)
		|| m_lava_03.collision(m_player_box)
		|| m_lava_04.collision(m_player_box)
		|| m_lava_05.collision(m_player_box)
		|| m_lava_06.collision(m_player_box)
		)
	{
		m_player.take_damage(time_step);
	}

	hud.on_update(time_step, m_player.hearts());

	m_cross_fade->on_update(time_step);
	m_ring.on_update(time_step, m_player.position());

	m_audio_manager->update_with_camera(m_3d_camera);
}

void level::on_render()
{
	engine::render_command::clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
	engine::render_command::clear();
	// Set up text shader
	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	// Set up  shader. (renders textures and materials)
	const auto mesh_shader = engine::renderer::shaders_library()->get("mesh");

	// Render 2D Camera
	engine::renderer::begin_scene(m_2d_camera, mesh_shader);
	if (m_state == level::MENU)
		m_game_intro->on_render(mesh_shader);
	else if (m_state == level::GAME_WON)
		m_game_won->on_render(mesh_shader);
	else if (m_state == level::GAME_LOST)
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
	for (auto coin : m_coin_list)
	{
		coin->on_render(mesh_shader);
	}

	// Render all of the decorations in the map.
	m_decorations.on_render(mesh_shader);

	// Render the player's character and enemies
	m_player.on_render(mesh_shader);
	for (auto enemy : m_skeleton_list)
		enemy->on_render(mesh_shader, m_3d_camera);
	for (auto enemy : m_robot_list)
		enemy->on_render(mesh_shader, m_3d_camera);
	for (auto enemy : m_crab_list)
		enemy->on_render(mesh_shader, m_3d_camera);
	for (auto enemy : m_spike_list)
		enemy->on_render(mesh_shader);

	m_mannequin_material->submit(mesh_shader);
	m_enemy_mech.on_render(mesh_shader, m_3d_camera);


	engine::renderer::end_scene();

	engine::renderer::begin_scene(m_2d_camera, mesh_shader);
	hud.on_render(mesh_shader);
	engine::renderer::end_scene();

	engine::renderer::begin_scene(m_2d_camera, mesh_shader);
	m_cross_fade->on_render(mesh_shader);
	engine::renderer::end_scene();

	// Render text
	m_text_manager->render_text(text_shader, std::to_string(m_player.coins()), 43.f, (float)engine::application::window().height() - 91.f, 0.5f, glm::vec4(1.f, 0.85f, 0.f, 1.f));
	m_text_manager->render_text(text_shader, std::to_string(m_reached_time == 0.f ? (int)m_remained_time : (int)m_reached_time) + "s", 45.f, (float)engine::application::window().height() - 135.f, 0.5f, glm::vec4(.85f, 0.85f, 0.85f, 1.f));
}

void level::on_event(engine::event& event)
{
	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);
		if (m_state == level::MENU && e.key_code() == engine::key_codes::KEY_ENTER)
		{
			m_state = level::IN_GAME;
			m_audio_manager->stop("menu");
			m_audio_manager->play("main");
			m_audio_manager->volume("main", 0.05f);
			m_play_time.start();
		}
		if ((m_state == level::GAME_LOST || m_state == level::GAME_WON) && e.key_code() == engine::key_codes::KEY_ESCAPE)
		{
			engine::application::exit();
		}

		if (e.key_code() == engine::key_codes::KEY_TAB)
		{
			engine::render_command::toggle_wireframe();
		}
	}
}
