#include "enemy_basic_robot.h"
#include "platform/opengl/gl_shader.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

enemy_basic_robot::enemy_basic_robot(glm::vec3 position) : m_position(position)
{

	std::vector<std::pair<std::string, int>> animations =
	{
		std::pair<std::string, int>("walk", 8),
		std::pair<std::string, int>("attack", 0),
		std::pair<std::string, int>("run", 8),
		std::pair<std::string, int>("die", 4)
	};

	// Free model from here : https://poly.pizza/m/yq5ATpujSt
	engine::ref<engine::skinned_mesh> m_enemy_mesh = engine::skinned_mesh::create("assets/models/animated/Two_Legs.fbx");
	m_enemy_mesh->switch_root_movement(false);
	m_enemy_mesh->switch_animation(3);

	engine::game_object_properties skeleton_props;
	skeleton_props.animated_mesh = m_enemy_mesh;
	engine::ref<engine::texture_2d> skeleton_texture =
		engine::texture_2d::create("assets/textures/Characters_Brown.png", true);
	skeleton_props.textures = { skeleton_texture };
	skeleton_props.mass = 27.2f;
	skeleton_props.velocity = glm::vec3(0.f);
	skeleton_props.scale = glm::vec3(.7f);
	skeleton_props.bounding_shape = glm::vec3(m_enemy_mesh->size().x * skeleton_props.scale.x,
		m_enemy_mesh->size().y * skeleton_props.scale.x * 2.f, m_enemy_mesh->size().z * skeleton_props.scale.x);

	m_object = engine::game_object::create(skeleton_props);

	m_object->set_position(m_position);

	m_enemy_basic.initialise(m_object, 4.f, animations);
};

enemy_basic_robot::~enemy_basic_robot() {};

void enemy_basic_robot::on_initialize()
{

};

void enemy_basic_robot::on_update(const engine::timestep& time_step, player& player, const engine::bounding_box player_box)
{
	m_enemy_basic.on_update(time_step, player, player_box, player.object()->position());
};

void enemy_basic_robot::on_render(const engine::ref<engine::shader> mesh_shader, const engine::perspective_camera& camera)
{
	m_enemy_basic.on_render(mesh_shader, camera);
};

engine::ref<enemy_basic_robot> enemy_basic_robot::create(glm::vec3 position)
{
	return std::make_shared<enemy_basic_robot>(position);
}

