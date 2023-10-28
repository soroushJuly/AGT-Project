#pragma once

namespace engine
{
	class mesh;

	class coin
	{
	public:
		coin();
		~coin();

		// getters
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_coin_mesh; }

		static ref<coin> create();
	private:
		ref<engine::mesh> m_coin_mesh;
		std::vector<glm::vec3> m_vertices;

		float m_size;
		float m_radius;
	};
}
