#pragma once
#include <engine.h>

class static_object
{
public:
	static_object();
	~static_object();

	void on_initialize(const std::string& model_path, const std::string& texture_path,
		glm::vec3 position = glm::vec3(-1.f, 0.f, -10.f));
	// If translate provided:
	void on_render(engine::ref<engine::shader> mesh_shader, glm::vec3 translate,
		float rotation_amount = 0.f,
		glm::vec3 rotation_axis = glm::vec3(0.f, 1.f, 0.f),
		glm::vec3 scale = glm::vec3(.01f));
	// If translate not provided:
	void on_render(engine::ref<engine::shader> mesh_shader,
		float rotation_amount = 0.f,
		glm::vec3 rotation_axis = glm::vec3(0.f, 1.f, 0.f),
		glm::vec3 scale = glm::vec3(.01f));

private:
	engine::ref<engine::game_object>	m_object{};

	std::string m_model_path;
	std::string m_texture_path;
};
