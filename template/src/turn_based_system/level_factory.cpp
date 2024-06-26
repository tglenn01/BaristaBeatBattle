#pragma once

#include "level_factory.hpp"

const int VERTICAL_SPACE_BETWEEN_ALLIES = 150;
const int VERTICAL_SPACE_BETWEEN_ENEMIES = 150;

LevelFactory::LevelFactory() {
}
	

Level* LevelFactory::construct_level_one(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position) {
	Level* level_one = new Level();
	level_one->level_number = 1;

	Entity chai_entity = create_chai(renderer, base_ally_position);

	level_one->allies.push_back(chai_entity);

	Entity level_one_ground_a = create_turn_based_enemy(renderer, base_enemy_position, 0);

	level_one->enemies.push_back(level_one_ground_a);

	return level_one;
}

Level* LevelFactory::construct_level_two(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position) {

	Level* level_two = new Level();
	level_two->level_number = 2;

	Entity chai_entity = create_chai(renderer, base_ally_position);

	level_two->allies.push_back(chai_entity);

	Entity earl_entity = create_earl(renderer, base_enemy_position);
	Entity americano_entity = create_americano(renderer, { base_enemy_position.x,base_enemy_position.y - VERTICAL_SPACE_BETWEEN_ALLIES });

	level_two->enemies.push_back(earl_entity);
	level_two->enemies.push_back(americano_entity);


	return level_two;

}

Level* LevelFactory::construct_level_three(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position) {

	Level* level_three = new Level();
	level_three->level_number = 3;

	Entity chai_entity = create_chai(renderer, base_ally_position);
	Entity earl_entity = create_earl(renderer, { base_ally_position .x,base_ally_position.y - VERTICAL_SPACE_BETWEEN_ALLIES });
	Entity americano_entity = create_americano(renderer, { base_ally_position.x,base_ally_position.y - (VERTICAL_SPACE_BETWEEN_ALLIES * 2)});

	level_three->allies.push_back(chai_entity);
	level_three->allies.push_back(earl_entity);
	level_three->allies.push_back(americano_entity);

	Entity level_two_ground_a = create_turn_based_enemy(renderer, base_enemy_position, 2);
	Entity level_two_ground_b = create_turn_based_enemy(renderer, { base_enemy_position.x,base_enemy_position.y - VERTICAL_SPACE_BETWEEN_ENEMIES }, 2);

	level_three->enemies.push_back(level_two_ground_a);
	level_three->enemies.push_back(level_two_ground_b);

	return level_three;

}

Level* LevelFactory::construct_level_four(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position) {

	Level* level_four = new Level();
	level_four->level_number = 4;


	Entity chai_entity = create_chai(renderer, base_ally_position);
	Entity earl_entity = create_earl(renderer, { base_ally_position.x,base_ally_position.y - VERTICAL_SPACE_BETWEEN_ALLIES });
	Entity americano_entity = create_americano(renderer, { base_ally_position.x,base_ally_position.y - (VERTICAL_SPACE_BETWEEN_ALLIES * 2) });

	level_four->allies.push_back(chai_entity);
	level_four->allies.push_back(earl_entity);
	level_four->allies.push_back(americano_entity);

	// TO BECOME TEA ENEMIES
	Entity level_two_ground_a = create_turn_based_enemy(renderer, base_enemy_position, 2);
	//Entity level_two_ground_b = create_turn_based_enemy(renderer, { base_enemy_position.x,base_enemy_position.y - VERTICAL_SPACE_BETWEEN_ALLIES }, 2);
	Entity level_two_ground_c = create_turn_based_enemy(renderer, { base_enemy_position.x,base_enemy_position.y - (VERTICAL_SPACE_BETWEEN_ENEMIES * 2) }, 2);

	// temp here as we currently only have 5 levels
	Entity possessed_london = create_london(renderer, { base_enemy_position.x,base_enemy_position.y - VERTICAL_SPACE_BETWEEN_ENEMIES });

	level_four->enemies.push_back(level_two_ground_a);
	//level_four->enemies.push_back(level_two_ground_b);

	level_four->enemies.push_back(possessed_london);

	level_four->enemies.push_back(level_two_ground_c);


	return level_four;

}

Level* LevelFactory::construct_level_five(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position) {

	Level* level_five = new Level();
	level_five->level_number = 5;

	Entity chai_entity = create_chai(renderer, base_ally_position);
	Entity earl_entity = create_earl(renderer, { base_ally_position.x,base_ally_position.y - VERTICAL_SPACE_BETWEEN_ALLIES });
	Entity americano_entity = create_americano(renderer, { base_ally_position.x,base_ally_position.y - (VERTICAL_SPACE_BETWEEN_ALLIES * 2) });

	level_five->allies.push_back(chai_entity);
	level_five->allies.push_back(earl_entity);
	level_five->allies.push_back(americano_entity);

	// TO BECOME TEA ENEMIES
	Entity level_two_ground_a = create_turn_based_enemy(renderer, base_enemy_position, 2);
	Entity level_two_ground_b = create_turn_based_enemy(renderer, { base_enemy_position.x,base_enemy_position.y - (VERTICAL_SPACE_BETWEEN_ENEMIES * 2) }, 2);
	// To become devil, in the middle
	Entity ground_devil = create_devil(renderer, { base_enemy_position.x,base_enemy_position.y - VERTICAL_SPACE_BETWEEN_ENEMIES });

	level_five->enemies.push_back(level_two_ground_a);
	level_five->enemies.push_back(level_two_ground_b);
	level_five->enemies.push_back(ground_devil);

	if (flag_progression->is_london_recruited) {
		Entity london = create_london(renderer, { base_ally_position.x,base_ally_position.y - (VERTICAL_SPACE_BETWEEN_ALLIES * 3) });
		level_five->allies.push_back(london);
	}

	return level_five;

}



Level* LevelFactory::construct_level_six(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position) {

	Level* level_six = new Level();
	level_six->level_number = 6;

	Entity chai_entity = create_chai(renderer, base_ally_position);
	Entity earl_entity = create_earl(renderer, { base_ally_position.x,base_ally_position.y - VERTICAL_SPACE_BETWEEN_ALLIES });
	Entity americano_entity = create_americano(renderer, { base_ally_position.x,base_ally_position.y - (VERTICAL_SPACE_BETWEEN_ALLIES * 2) });

	level_six->allies.push_back(chai_entity);
	level_six->allies.push_back(earl_entity);
	level_six->allies.push_back(americano_entity);

	Entity level_three_ground_a = create_turn_based_enemy(renderer, base_enemy_position, 3);
	Entity level_three_ground_b = create_turn_based_enemy(renderer, { base_enemy_position.x,base_enemy_position.y - VERTICAL_SPACE_BETWEEN_ALLIES }, 3);

	level_six->enemies.push_back(level_three_ground_a);
	level_six->enemies.push_back(level_three_ground_b);

	return level_six;

}

Level* LevelFactory::construct_level_seven(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position) {

	Level* level_seven = new Level();
	level_seven->level_number = 7;

	Entity chai_entity = create_chai(renderer, base_ally_position);
	Entity earl_entity = create_earl(renderer, { base_ally_position.x,base_ally_position.y - VERTICAL_SPACE_BETWEEN_ALLIES });
	Entity americano_entity = create_americano(renderer, { base_ally_position.x,base_ally_position.y - (VERTICAL_SPACE_BETWEEN_ALLIES * 2) });

	level_seven->allies.push_back(chai_entity);
	level_seven->allies.push_back(earl_entity);
	level_seven->allies.push_back(americano_entity);

	// TO BECOME TEA ENEMIES
	Entity level_two_ground_a = create_turn_based_enemy(renderer, base_enemy_position, 2);
	Entity level_two_ground_b = create_turn_based_enemy(renderer, { base_enemy_position.x,base_enemy_position.y - (VERTICAL_SPACE_BETWEEN_ALLIES * 2) }, 2);
	// To become devil, in the middle
	Entity ground_devil = create_turn_based_enemy(renderer, { base_enemy_position.x,base_enemy_position.y - VERTICAL_SPACE_BETWEEN_ALLIES }, 5);

	level_seven->enemies.push_back(level_two_ground_a);
	level_seven->enemies.push_back(level_two_ground_b);
	level_seven->enemies.push_back(ground_devil);
	
	return level_seven;

}