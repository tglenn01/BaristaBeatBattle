#pragma once

#include "abilities.hpp"
#include "character_stat_sheet.hpp"
#include <vector>
#include <string>

// Have to forward declare Ability for some reason.
class Ability;

class Character {
public:
	
	Character(std::string name, CharacterStatSheet* stats);

	void add_ability(Ability* ability);

	void restore_health_to_full();

	unsigned int get_current_health_points();

	CharacterStatSheet* get_character_stat_sheet();

	bool is_dead();


private:
	std::string character_name;
	CharacterStatSheet* stats;
	std::vector<Ability*> abilities;
	std::vector<Character*> party_members;

	unsigned int current_health_points;
};