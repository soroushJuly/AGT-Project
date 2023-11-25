#pragma once

namespace engine
{
	class mesh;

	class heart
	{
	public:
		/// \brief Constructor
		heart(float size);
		/// \brief Destructor
		~heart();

		/// \brief Getter methods
		ref<engine::mesh> mesh() const { return m_heart_mesh; }

		static ref<heart> create(float size = 1.f);
	private:
		/// \brief Fields
		// vertices of the heart specified by a vector of positions.
		float m_size;
		ref<engine::mesh> m_heart_mesh;
	};
}
