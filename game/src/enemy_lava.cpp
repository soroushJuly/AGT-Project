#include "enemy_lava.h"

enemy_lava::enemy_lava()
{
}

enemy_lava::~enemy_lava()
{
}

void enemy_lava::on_initialise(glm::vec3 position, glm::vec3 half_extends, float rotation)
{
	std::vector<engine::ref<engine::texture_2d>> lava_textures = { engine::texture_2d::create("assets/textures/lava_02.png", false) };
	engine::game_object_properties lava_props;
	engine::ref<engine::cuboid> lava_shape = engine::cuboid::create(half_extends, false, 4);
	lava_props.meshes = { lava_shape->mesh() };
	lava_props.rotation_amount = rotation;
	lava_props.textures = { lava_textures };
	lava_props.position = position;
	lava_props.is_static = true;
	m_lava = engine::game_object::create(lava_props);

	m_lava_box.set_box(lava_shape->half_extents().x * 2.f, .15f, lava_shape->half_extents().z * 2.f, position);
}

void enemy_lava::on_render(const engine::ref<engine::shader> mesh_shader)
{
	engine::renderer::submit(mesh_shader, m_lava);
}

bool enemy_lava::collision(engine::bounding_box other_box)
{
	return m_lava_box.collision(other_box);
}

