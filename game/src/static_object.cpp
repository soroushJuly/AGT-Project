#include <static_object.h>
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/utils/track.h"

static_object::static_object() {}
static_object::~static_object() {}


void static_object::on_initialize(const std::string& model_path, const std::string& texture_path)
{
	engine::ref <engine::model> model = engine::model::create(model_path);
	std::vector<engine::ref<engine::texture_2d>> tree_texture = { engine::texture_2d::create(texture_path, false) };
	engine::game_object_properties model_props;
	model_props.meshes = model->meshes();
	model_props.textures = tree_texture;
	model_props.position = { 4.f, 0.5f, -5.f };
	model_props.scale = glm::vec3(.01f);
	m_object = engine::game_object::create(model_props);
}

void static_object::on_render(engine::ref<engine::shader> mesh_shader)
{
	glm::mat4 object_transform(1.0f);

	object_transform = glm::translate(object_transform, glm::vec3(4.f, 0.5, -5.0f));
	object_transform = glm::rotate(object_transform, m_object->rotation_amount(), m_object->rotation_axis());
	object_transform = glm::scale(object_transform, glm::vec3(.01f));
	engine::renderer::submit(mesh_shader, object_transform, m_object);

}
