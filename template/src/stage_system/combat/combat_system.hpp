#pragma once
#include "stage_system/stage_system.hpp"
#include "turn_based_system/level_factory.hpp"

class CombatSystem
{
public: 
	CombatSystem(); 

	// Can add more levels to level enum here
	enum CombatLevel {
		level_one, 
		level_two,
		level_three, 
		level_four,
		level_five,
		level_six,
		level_seven
	};

	enum SoundMapping {
		change_sound, 
		attack_sound, 
		no_sound
	};

	void init(StageSystem* stage_system_arg, TurnBasedSystem* turn_based_arg);

	SoundMapping handle_turnbased_keys(int key, int action);

	void handle_level(RenderSystem* renderer);

	void handle_turn_rendering();

private:
	void init_combat_data_for_level(RenderSystem* renderer, Level* level);

	void handle_tutorial();

	void handle_combat_over();

	SoundMapping handle_menu(int key);

	SoundMapping handle_attack(Entity active_char_entity, std::string ability);

	SoundMapping handle_selection();
	
	StageSystem* stage_system;
	CombatLevel selected_level;
	TurnBasedSystem* turn_based;
	LevelFactory* level_factory;
	bool turn_based_tutorial;
};