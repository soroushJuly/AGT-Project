#pragma once

namespace engine
{
	class mesh;

	class arrow
	{
	public:
		arrow(float size);
		~arrow();

		static engine::ref<engine::arrow> engine::arrow::create(float size = 1.f);
		engine::ref<engine::mesh> mesh() const { return m_arrow_mesh; }

	private:
		engine::ref<engine::mesh> m_arrow_mesh;
		float m_size;
		float m_depth;
	};
}

