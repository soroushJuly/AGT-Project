#include "pickup.h"

pickup::pickup(const engine::game_object_properties props) : engine::game_object(props)
{}
pickup::~pickup()
{}
void pickup::init()
{
	m_is_active = true;
}
void pickup::deactive()
{
	m_is_active = false;
}
void pickup::update(glm::vec3 c, float dt)
{
	set_rotation_amount(rotation_amount() + dt);

	float distance = glm::length(c - position());
	if (distance < 1.f)
	{
		deactive();
	}
	//else if (!active())
	//{
	//	init();
	//}
}
engine::ref<pickup> pickup::create(const engine::game_object_properties& props)
{
	return std::make_shared<pickup>(props);
}
