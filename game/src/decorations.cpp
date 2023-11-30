#include "decorations.h"

decorations::decorations()
{
}

decorations::~decorations()
{
}

void decorations::on_initialise()
{
	// Textures below from POLYGON Pack https://syntystore.com/products/polygon-adventure-pack?_pos=1&_psq=adve&_ss=e&_v=1.0
	// Decorations initilize
	campfire.on_initialize("assets/models/static/SM_Env_CampFire_01.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(1.f, .5f, 10.f));
	bush.on_initialize("assets/models/static/SM_Env_Bush_01.fbx", "assets/textures/grass.png", glm::vec3(4.f, .45f, 23.5f));
	tree_01.on_initialize("assets/models/static/SM_Env_Tree_02.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(4.f, 0, 22.f));
	tree_02.on_initialize("assets/models/static/SM_Env_Tree_04.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(-4.f, 0, 22.f));
	tree_03.on_initialize("assets/models/static/SM_Env_Tree_08.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(-4.f, 0, 24.f));
	tree_04.on_initialize("assets/models/static/SM_Env_Tree_09.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(-4.f, 0, 24.f));
	fence_01.on_initialize("assets/models/static/SM_Bld_Fence_01.fbx", "assets/textures/PolyAdventureTexture_01.png");
	cart_01.on_initialize("assets/models/static/SM_Prop_Cart_01.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(6.f, .5f, 74.f));
	hut_01.on_initialize("assets/models/static/SM_Bld_Village_02.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(1.f, .5f, 26.f));
	hut_02.on_initialize("assets/models/static/SM_Bld_Village_06.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(1.f, .5f, 26.f));
	hut_03.on_initialize("assets/models/static/SM_Bld_Village_07.fbx", "assets/textures/PolyAdventureTexture_01.png", glm::vec3(1.f, .5f, 26.f));
}

void decorations::on_render(const engine::ref<engine::shader> mesh_shader)
{
	// Render all of the static decoration in game here:
	// Box 1 decorations
	campfire.on_render(mesh_shader, 0.f, campfire.object()->rotation_axis(), glm::vec3(0.006f));
	bush.on_render(mesh_shader);
	bush.on_render(mesh_shader, glm::vec3(4.f, .45f, 16.25f), engine::PI / 2, glm::vec3(0.f, 1.f, 0.f), glm::vec3(.013f, .01f, .013f));
	tree_01.on_render(mesh_shader);
	tree_02.on_render(mesh_shader);
	tree_03.on_render(mesh_shader);
	tree_03.on_render(mesh_shader, glm::vec3(-4.f, 0, 17.f), 0.f, glm::vec3(0.f, 1.f, 0.f), glm::vec3(.013f, .018f, .013f));
	tree_03.on_render(mesh_shader, glm::vec3(-4.3f, 0, 20.3f), engine::PI, glm::vec3(0.f, 1.f, 0.f), glm::vec3(.013f));

	// Box 2 decoratinos
	for (int i = 0; i < 16; i++)
	{
		fence_01.on_render(mesh_shader, glm::vec3(4.2f, .5f, 26.f + (float)i * 3.f), engine::PI / 2);
		fence_01.on_render(mesh_shader, glm::vec3(-4.2f, .5f, 26.f + (float)i * 3.f), engine::PI / 2);
	}
	//fence_02.on_render(mesh_shader, engine::PI / 2);

	// Box 3 decorations
	for (int i = 0; i < 13; i++)
	{
		tree_03.on_render(mesh_shader, glm::vec3(-4.2f, .5f, 75.f + (float)i * 2.7f), 0.f, glm::vec3(0.f, 1.f, 0.f),
			tree_03.object()->scale() + (float)(i % 3) / 150.f);
		//fence_01.on_render(mesh_shader, glm::vec3(-4.2f, .5f, 26.f + (float)i * 3.f), engine::PI / 2);
	}
	cart_01.on_render(mesh_shader, engine::PI / 4);

	// Box 4 decorations
	for (int i = 0; i < 20; i++)
	{
		fence_01.on_render(mesh_shader, glm::vec3(22.2f + (float)i * 3.f, .5f, 102.f));
		fence_01.on_render(mesh_shader, glm::vec3(22.2f + (float)i * 3.f, .5f, 95.f));
	}

	// Box 5 decorations
	for (int i = 0; i < 18; i++)
	{
		fence_01.on_render(mesh_shader, glm::vec3(97.5f, .5f, 36.f + (float)i * 3.f), engine::PI / 2);
		fence_01.on_render(mesh_shader, glm::vec3(89.5f, .5f, 36.f + (float)i * 3.f), engine::PI / 2);
	}
	for (int i = 0; i < 10; i++)
	{
		tree_04.on_render(mesh_shader, glm::vec3(99.5f, .5f, 36.f + (float)i * 5.2f));
		tree_04.on_render(mesh_shader, glm::vec3(87.5f, .5f, 36.f + (float)i * 5.2f));
	}

	// Box 6 decoration
	cart_01.on_render(mesh_shader, glm::vec3(69.f, .5f, 20.f));
	hut_01.on_render(mesh_shader, glm::vec3(67.f, .5f, 28.f));
	hut_02.on_render(mesh_shader, glm::vec3(62.f, .5f, 21.f));
	hut_03.on_render(mesh_shader, glm::vec3(66.f, .5f, 9.f));
	hut_01.on_render(mesh_shader, glm::vec3(66.f, .5f, 14.f));
}
