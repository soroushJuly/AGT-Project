#pragma once
#include <static_object.h>

// A class to render all static objects (without collisions) in the world.
class decorations
{
public:
	decorations();
	~decorations();

	void on_initialise();
	void on_render(const engine::ref<engine::shader> mesh_shader);

private:

	static_object cart_01{};
	static_object fence_01{};
	static_object fence_02{};
	static_object bush{};
	static_object campfire{};
	static_object tree_01{};
	static_object tree_02{};
	static_object tree_03{};
	static_object tree_04{};
	static_object hut_01{};
	static_object hut_02{};
	static_object hut_03{};

};


