#pragma once

namespace engine
{
	class mesh;

	class spike
	{
	public:
		spike(float size);
		~spike();

		static engine::ref<engine::spike> create(float size = 1.f);
		engine::ref<engine::mesh> mesh() const { return m_spike_mesh; }

	private:
		engine::ref<engine::mesh> m_spike_mesh;
		float m_size;

	};
}
