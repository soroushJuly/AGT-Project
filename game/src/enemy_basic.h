#pragma once
#include "engine.h"
#include "engine/entities/bounding_box.h"
#include "FX/billboard.h"
#include "player.h"

class player;

class enemy_basic
{

	enum states
	{
		WANDER,
		CHASE_RUN,
		CHASE_WALK,
		ATTACK
	};

	int m_state = WANDER;
public:
	enemy_basic();
	~enemy_basic();

	void initialise(engine::ref<engine::game_object> object, float wander_limit, std::vector<std::pair<std::string, int>> animations);
	void on_update(const engine::timestep& time_step, player& player, engine::bounding_box m_player_box, const glm::vec3& target_position);
	void on_render(const engine::ref<engine::shader> mesh_shader, const engine::perspective_camera& camera);
	void take_damage();
	void run();
	void walk();
	void die();


	// states related methods
	void wander(const engine::timestep& time_step);
	void chase_enemy_run(const engine::timestep& time_step, const glm::vec3& target_position);
	void chase_enemy_walk(const engine::timestep& time_step, const glm::vec3& target_position);
	void attack(const engine::timestep& time_step, player& player, engine::bounding_box m_player_box);
	void chase_target(const engine::timestep& time_step, const glm::vec3& target_position);


	engine::ref<engine::game_object> object() const { return m_object; }
private:
	int m_health{ 2 };
	float m_damage_timer;
	float m_speed;
	bool m_is_dead{ false };
	bool m_is_dying{ false };
	float m_timer;
	float m_angle{ 0.f };

	float m_attack_timer;

	int m_run_animation;
	int m_die_animation;
	int m_walk_animation;
	int m_attack_animation;

	float max_velocity;

	// probably should use these variables to control the character
	bool m_is_punching{ false };
	bool is_walking{ false }, is_running{ false }, is_standing{ false };
	engine::ref< engine::game_object> m_object;
	float m_contact_time;
	glm::vec3 m_instantaneous_acceleration{ 0.f };

	float m_wander_limit;
	glm::vec3 m_first_position;
	engine::bounding_box m_enemy_box;

	engine::ref<billboard>				m_billboard{};
};

