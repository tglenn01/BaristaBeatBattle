#include "turn_based_system.hpp"
#include "./character_factory/character_factory.hpp"


#include "common.hpp"
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <iostream>

const unsigned int SPEED_REQUIRED_FOR_TURN = 100;
const double HIT_CHANCE = 0.85; // 0.85

TurnBasedSystem::TurnBasedSystem() {
	// stub;
	// influence for random code
	// https://www.geeksforgeeks.org/generate-a-random-number-between-0-and-1/
	srand(time(0));

}

void TurnBasedSystem::init(AISystem* ai_system) {
	this->ai_system = ai_system;
	for (int i = (int)registry.players.components.size() - 1; i >= 0; --i) {
		std::cout << "I GOT HEREEEEE" << std::endl;
		current_level = registry.players.components[0].level_num;
	}
	
}

void TurnBasedSystem::step(float elapsed_ms_since_last_update) {

	if (out_of_combat || waiting_for_player) return;

	if (active_character == emptyEntity) {

		TurnCounter* highest = NULL;
		Entity highest_entity_number;

		for (Entity turnCounterEntity : registry.turnCounter.entities) {

			TurnCounter* tc = registry.turnCounter.get(turnCounterEntity);

			tc->placement += tc->speed_value;

			if (highest == NULL || highest->placement < tc->placement) {
				highest = tc;
				highest_entity_number = turnCounterEntity;
			}

		}

		assert(highest != NULL && "There ara no character in the turn counter");

		if (highest->placement < SPEED_REQUIRED_FOR_TURN) return;

		active_character = highest_entity_number;
		highest->placement -= 100;

	}


	if (registry.partyMembers.has(active_character)) {
		waiting_for_player = true;

	}
	else {

		double duration;

		if (enemy_await == NULL) {
			enemy_await = std::clock();
		}

		duration = (std::clock() - enemy_await) / (double)CLOCKS_PER_SEC;

		if (duration < 0.25) return;

		Character* ai_character = registry.characterDatas.get(active_character).characterData;


		Character* target_character = ai_system->ai_find_target();

		std::cout << ai_character->get_name() << "'s targeting " << target_character->get_name() << " best they have the lowest health!" << "\n";

		int outcome = process_character_action(ai_character->get_ability_by_name("Basic Attack"), ai_character, { target_character});
	
		enemy_await = NULL;

	}

	
}


void TurnBasedSystem::start_encounter(Level* level) {
	registry.partyMembers.clear();
	registry.turnBasedEnemies.clear();
	registry.turnCounter.clear();


	for (Entity ally_entity : level->allies) {
		PartyMember partyMemberComponent = PartyMember();
		registry.partyMembers.emplace(ally_entity, partyMemberComponent);

		Character* characterData = registry.characterDatas.get(ally_entity).characterData;
		characterData->restore_health_to_full();

		TurnCounter* turn = new TurnCounter();

		turn->placement = 0;
		turn->speed_value = characterData->get_character_stat_sheet()->get_speed();

		registry.turnCounter.emplace(ally_entity, turn);

		
	}

	for (Entity enemy_entity : level->enemies) {
		Character* characterData = registry.characterDatas.get(enemy_entity).characterData;

		TurnBasedEnemy tbe = TurnBasedEnemy();
		tbe.experience_value = characterData->level;
		registry.turnBasedEnemies.emplace(enemy_entity, tbe);


		TurnCounter* turn = new TurnCounter();

		turn->placement = 0;
		turn->speed_value = characterData->get_character_stat_sheet()->get_speed();

		registry.turnCounter.emplace(enemy_entity, turn);


	}


	out_of_combat = false;
}

/*

0 = normal return state
1 = allies win
2 = enemies win

*/

int TurnBasedSystem::process_character_action(Ability* ability, Character* caller, std::vector<Character*> recipients) {

	std::cout << "Current Character: " << caller->get_name() << '\n';

		//ability->process_ability(caller, receiving_character);
	double chance_hit = ((double)rand()) / RAND_MAX;
	Entity caller_entity = get_entity_given_character(caller);
	if (ability->get_ability_name() == "rest" || chance_hit < HIT_CHANCE) {
		
		if (ability->get_ability_name() != "rest") {
			if (registry.attackTimers.has(caller_entity)) {
				// registry.injuryTimers.remove(receiving_entity);
				AttackTimer& attack = registry.attackTimers.get(caller_entity);
				attack.counter_ms = 700.f;
			}
			else {
				registry.attackTimers.emplace(caller_entity);
			}
		}
		
		for (Character* receiving_character : recipients) {

			ability->process_ability(caller, receiving_character);
			Entity receiving_entity = get_entity_given_character(receiving_character);
			if (ability->get_ability_name() != "rest") {
				if (registry.injuryTimers.has(receiving_entity)) {
					// registry.injuryTimers.remove(receiving_entity);
					InjuredTimer& injury = registry.injuryTimers.get(receiving_entity);
					injury.counter_ms = 3000.f;
					injury.redness_factor = 1.f;
				}
				else {
					InjuredTimer& injury = registry.injuryTimers.emplace(receiving_entity);
					injury.counter_ms = 3000.f;
					//std::cout << "Current time: " << (3000.f - registry.injuryTimers.get(receiving_entity).counter_ms) /3000.f << '\n';
				}
			}
			

			HealthBarFill& fill = get_health_bar_given_entity(receiving_entity);
			fill.percent_filled = (float) receiving_character->get_current_health_points() / receiving_character->get_character_stat_sheet()->get_max_health();
			std::cout << "Current percent health: " << receiving_character->get_current_health_points() / receiving_character->get_character_stat_sheet()->get_max_health() << '\n';
			std::cout << "Current health: " << receiving_character->get_current_health_points()  << '\n';
			std::cout << "Current health: " << receiving_character->get_character_stat_sheet()->get_max_health() << '\n';

			if (receiving_character->is_dead()) {


				process_death(get_entity_given_character(receiving_character));


			}

		}
	}
	else {
		std::cout << caller->get_name() << " Missed!" << '\n';
		if (!registry.missTimers.has(caller_entity)) {
			MissTimer& miss = registry.missTimers.emplace(caller_entity);
			Motion& caller_motion = registry.motions.get(caller_entity);
			Entity miss_text = createText("MISS!", vec2(caller_motion.position.x + 50.f, window_height_px - caller_motion.position.y), 0.7f, vec3(1.0f, 0.0f, 0.0f), mat4(1.0f), StageSystem::Stage::turn_based, false);
			registry.textRenderRequests.get(miss_text).shown = true;
			miss.associated_text = miss_text;
		}
	}

	active_character = emptyEntity;
	waiting_for_player = false;


	if (are_all_allies_defeated()) {
		out_of_combat = true;
		printf("Game Over! You lost :(");
		this->end_of_game_wait = std::clock();
		return 2;
	}

	if (are_all_enemies_defeated()) {
		out_of_combat = true;
		printf("Game Over! You won the fight!!");
		this->end_of_game_wait = std::clock();
		return 1;
	}

	if (registry.turnBasedEnemies.size() == 1) {
		Entity turn_based_enemy_entity = registry.turnBasedEnemies.entities[0];
		Character* turn_based_enemy = registry.characterDatas.get(turn_based_enemy_entity).characterData;
		if (turn_based_enemy->get_name() == "London") {
			out_of_combat = true;
			printf("London was the last enemy alive and has been recruited!");
			flag_progression->is_london_recruited = true;
			this->end_of_game_wait = std::clock();
			return 1;
		}
	}

	return 0;

}




bool TurnBasedSystem::are_all_allies_defeated() {
	for (Entity partyMember : registry.partyMembers.entities) {
		Character* characterData = registry.characterDatas.get(partyMember).characterData;

		if (!characterData->is_dead()) {
			return false;
		}
	}
	return true;
}

bool TurnBasedSystem::are_all_enemies_defeated() {
	for (Entity enemyEntity : registry.turnBasedEnemies.entities) {
		Character* characterData = registry.characterDatas.get(enemyEntity).characterData;

		if (!characterData->is_dead()) {
			return false;
		}
	}
	return true;
}

void TurnBasedSystem::process_death(Entity o7)
{

	if (registry.turnBasedEnemies.has(o7)) {

		// apply experience to allies!
		for (Entity party_member : registry.partyMembers.entities) {			
			experience_manager.apply_experience(party_member, registry.turnBasedEnemies.get(o7).experience_value);
		}

		registry.turnBasedEnemies.remove(o7);
		registry.characterDatas.remove(o7);
	}

	if (registry.partyMembers.has(o7)) {
		registry.partyMembers.remove(o7);
	}

	registry.turnCounter.remove(o7);

}

Entity TurnBasedSystem::get_entity_given_character(Character* receiving_character)
{

	for (int i = 0; i < registry.characterDatas.size(); i++) {

		if (registry.characterDatas.components.at(i).characterData == receiving_character) {
			return registry.characterDatas.entities.at(i);
		}

	}
	return emptyEntity;
}


bool out_of_combat = true;

HealthBarFill& TurnBasedSystem::get_health_bar_given_entity(Entity receiving_character) {
	HealthBarFill dummy;
	for (Entity fills : registry.healthBarFills.entities) {
		HealthBarFill& fill = registry.healthBarFills.get(fills);
		if (fill.associated_char == receiving_character) {
			return fill;
		}
	}
	return dummy;
}