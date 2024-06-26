#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"
#include <iostream>


Entity createChicken(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = mesh.original_size * 300.f;
	motion.scale.y *= -1; // point front to the right

	registry.overWorld.emplace(entity);
	registry.players.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PLAYER, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::CHICKEN, // shuold prob fix this later
			GEOMETRY_BUFFER_ID::PLAYER });
	rr.isStatic = true;

	return entity;
}

Entity createMainMini(RenderSystem* renderer, vec2 pos, float rhythm_length, float inter_timer, std::string sprite) {
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = pos;

	// Place in minigame
	registry.miniGame.emplace(entity);
	// Place in minigame timer for ryhthym calcs
	MiniGameTimer& mgt = registry.miniGameTimer.emplace(entity);
	mgt.counter_ms = rhythm_length; 
	mgt.inter_timer = inter_timer;

	switch (sprite[0]) {
	// cup case
	case 'c': {
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::MINIGAMECUP,
			EFFECT_ASSET_ID::BATTLE,
			GEOMETRY_BUFFER_ID::SPRITE }
		);

		// Setting initial values, scale is negative to make it face the opposite way
		motion.scale = vec2({ -CUP_WIDTH, CUP_HEIGHT });
		break;
	}
	// kettle case
	case 'k': {
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::KETTLENORM,
			EFFECT_ASSET_ID::BATTLE,
			GEOMETRY_BUFFER_ID::SPRITE }
		);
		// Setting initial values, scale is negative to make it face the opposite way
		motion.scale = vec2({ KETTLE_WIDTH, KETTLE_HEIGHT });
		break;
	}
	case 'm': {
		registry.motions.remove(entity);
		break;
	}
	}

	return entity;
}

Entity createSpeech(RenderSystem* renderer, vec2 pos) {
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = pos;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ SPEECH_WIDTH, SPEECH_HEIGHT });

	// Place in minigame
	registry.miniGame.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MINISPEECH,
		EFFECT_ASSET_ID::BATTLE,
		GEOMETRY_BUFFER_ID::SPRITE }
	);

	return entity;
}

Entity createMiniResult(RenderSystem* renderer, vec2 pos, float interpolate_counter, minigame_state mini_result) {
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = pos;

	motion.scale = vec2({ MENU_WIDTH, MENU_HEIGHT });
	// place in minigame result
	MiniGameResTimer& res = registry.miniGameResTimer.emplace(entity);
	res.counter_ms = interpolate_counter;

	switch (mini_result) {
	case minigame_state::perfect:
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::MINIGAMEPERFECT,
			EFFECT_ASSET_ID::BATTLE,
			GEOMETRY_BUFFER_ID::SPRITE }
		);
		break;
	case minigame_state::good:
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::MINIGAMEGOOD,
			EFFECT_ASSET_ID::BATTLE,
			GEOMETRY_BUFFER_ID::SPRITE }
		);
		break;
	case minigame_state::fail:
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::MINIGAMEFAIL,
			EFFECT_ASSET_ID::BATTLE,
			GEOMETRY_BUFFER_ID::SPRITE }
		);
		break;
	case minigame_state::normal:
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::MINIGAMECOOLCLOUD,
			EFFECT_ASSET_ID::BATTLE,
			GEOMETRY_BUFFER_ID::SPRITE }
		);
		motion.scale = { motion.scale.x * 2, motion.scale.y * 2 };
		break;
	}
	res.res_state = mini_result;

	return entity;
}

// minigame hit indicator 
Entity createMiniIndicator(RenderSystem* renderer, vec2 pos, minigame_state mini_res) {
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = pos;

	motion.scale = vec2({ MENU_WIDTH, MENU_HEIGHT });

	MiniGameVisualizer& visual = registry.miniGameVisual.emplace(entity);
	visual.res_state = mini_res;

	if (mini_res == minigame_state::good) {
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::MINIGAMECOOLGOOD,
			EFFECT_ASSET_ID::BATTLE,
			GEOMETRY_BUFFER_ID::SPRITE }
		);
	}
	else if (mini_res == minigame_state::perfect) {
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::MINIGAMECOOLPERFECT,
			EFFECT_ASSET_ID::BATTLE,
			GEOMETRY_BUFFER_ID::SPRITE } 
		);
	}

	return entity;
}

// create milk option that attaches to a mc option
Entity createMilk(RenderSystem* renderer, vec2 pos, std::string milk_type) {
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = pos;

	motion.scale = vec2({ MENU_WIDTH, MENU_HEIGHT });

	// add entity to minigame component
	registry.miniGame.emplace(entity);

	switch (milk_type[0]) {
	case 'A': {
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::MILKALMOND,
			EFFECT_ASSET_ID::BATTLE,
			GEOMETRY_BUFFER_ID::SPRITE }
		);
		break;
	}
	case 'B': {
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::MILK2,
			EFFECT_ASSET_ID::BATTLE,
			GEOMETRY_BUFFER_ID::SPRITE }
		);
		break;
	}
	case 'C': {
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::MILKCOCONUT,
			EFFECT_ASSET_ID::BATTLE,
			GEOMETRY_BUFFER_ID::SPRITE }
		);
		break;
	}
	case 'D': {
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::MILKSOY,
			EFFECT_ASSET_ID::BATTLE,
			GEOMETRY_BUFFER_ID::SPRITE }
		);
		break;
	}
	}

	registry.renderRequests.get(entity).shown = true;

	return entity;
}

Entity createEnemyDrink(RenderSystem* renderer, vec2 velocity, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = velocity;
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -EAGLE_BB_WIDTH, EAGLE_BB_HEIGHT });

	registry.overWorld.emplace(entity);
	registry.enemyDrinks.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ENEMYDRINK,
		 EFFECT_ASSET_ID::FOREGROUND,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;

	return entity;
}

Entity createLevelNode(RenderSystem* renderer, int level_num, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = vec2(0.f,0.f);
	motion.position = position + vec2(0, 100);


	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -100, 100});

	registry.overWorld.emplace(entity);
	auto& levelNode = registry.levelNode.emplace(entity);
	levelNode.position = position;
	levelNode.level_number = level_num;


	// Based on the value of level_num, set the appropriate TEXTURE_ASSET_ID
	TEXTURE_ASSET_ID textureId;
	switch (level_num) {
	case 1:
		textureId = TEXTURE_ASSET_ID::LEVEL1;
		break;
	case 2:
		textureId = TEXTURE_ASSET_ID::LEVEL2;
		break;
	case 3:
		textureId = TEXTURE_ASSET_ID::LEVEL3;
		break;
	case 4:
		textureId = TEXTURE_ASSET_ID::LEVEL4;
		break;
	case 5:
		textureId = TEXTURE_ASSET_ID::LEVEL5;
		break;
		// Add more cases if there are more levels
	default:
		textureId = TEXTURE_ASSET_ID::LEVEL1; // Or some default case
		std::cerr << "Warning: Level number " << level_num << " not recognized. Using default texture." << std::endl;
		break;
	}


	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ textureId,
		 EFFECT_ASSET_ID::FOREGROUND,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;


	Entity level_text = createText(std::to_string(level_num), vec2(position.x - 90, 900 - position.y + 80), 1, glm::vec3(1.0f, 1.0f, 1.0f), glm::mat4(1.0f), StageSystem::Stage::overworld, false);
	registry.textRenderRequests.get(level_text).shown = true;

	return entity;
}

Entity createMenu(RenderSystem* renderer, vec2 pos, Entity associated_character)
{
	auto menuEnt = Entity();
	auto attack = Entity();
	auto rest = Entity();
	auto pourIt = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& atkMesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(attack, &atkMesh);
	Mesh& restMesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(rest, &restMesh);
	Mesh& pourItMesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(pourIt, &pourItMesh);

	MenuOption& pourItOp = registry.menuOptions.emplace(pourIt);
	registry.colors.insert(pourIt, { 1, 0.8f, 0.8f });
	pourItOp.option = "pour it";
	MenuOption& attackOp = registry.menuOptions.emplace(attack);
	registry.colors.insert(attack, { 1, 0.8f, 0.8f });
	attackOp.option = "attack";
	MenuOption& restOp = registry.menuOptions.emplace(rest);
	registry.colors.insert(rest, { 1, 0.8f, 0.8f });
	restOp.option = "rest";

	Menu& menu = registry.menu.emplace(menuEnt);
	menu.options[0] = attack;
	menu.options[1] = rest;
	menu.options[2] = pourIt;
	menu.activeOption = attack;

	menu.associated_character = associated_character;

	vec2 menuPos = pos;
	// Initialize the position, scale, and physics components
	auto& pourItMotion = registry.motions.emplace(pourIt);
	pourItMotion.angle = 0.f;
	pourItMotion.velocity = { 0, 0 };
	pourItMotion.position = { menuPos.x, menuPos.y - 108.f };
	pourItMotion.scale = vec2({ MENU_WIDTH, MENU_HEIGHT });

	auto& atkMotion = registry.motions.emplace(attack);
	atkMotion.angle = 0.f;
	atkMotion.velocity = { 0, 0 };
	// I am aware the 54 is a magic floating number, not sure how to calculate it as of rn
	atkMotion.position = { menuPos.x, menuPos.y - 54.f };
	atkMotion.scale = vec2({ MENU_WIDTH, MENU_HEIGHT });

	auto& restMotion = registry.motions.emplace(rest);
	restMotion.angle = 0.f;
	restMotion.velocity = { 0, 0 };
	restMotion.position = menuPos;
	restMotion.scale = vec2({ MENU_WIDTH, MENU_HEIGHT });

	registry.turnBased.emplace(menuEnt);
	registry.turnBased.emplace(attack);
	registry.turnBased.emplace(rest);
	registry.turnBased.emplace(pourIt);

	return menuEnt;
}

Entity createBackgroundScroller(RenderSystem* renderer, vec2 position) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// scale the background
	motion.scale = vec2({ BG_WIDTH, BG_HEIGHT });

	registry.overWorld.emplace(entity);
	registry.backgrounds.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BGSCROLL,
		 EFFECT_ASSET_ID::BACKGROUND,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.isStatic = true;

	return entity;
}

Entity createForegroundScroller(RenderSystem* renderer, vec2 position, bool isLight) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	registry.overWorld.emplace(entity);
	registry.foregrounds.emplace(entity);

	if (isLight) {
		// scale the background
		motion.scale = vec2({ BG_WIDTH, BG_HEIGHT });
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::FGLIGHT,
			 EFFECT_ASSET_ID::LIGHTS,
			 GEOMETRY_BUFFER_ID::SPRITE });
	}
	else {
		// scale the background
		motion.scale = vec2({ FG_WIDTH, FG_HEIGHT });
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::FGSCROLL,
			 EFFECT_ASSET_ID::BATTLE,
			 GEOMETRY_BUFFER_ID::SPRITE });
	}
	RenderRequest& rr = registry.renderRequests.get(entity);
	rr.isStatic = true;

	return entity;
}

Entity createBackgroundBattle(RenderSystem* renderer, vec2 position) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// scale the background
	motion.scale = vec2({ 1700, window_height_px });

	registry.turnBased.emplace(entity);
	registry.backgrounds.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BGBATTLE,
		 EFFECT_ASSET_ID::BATTLE,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.isStatic = true;

	return entity;
}


Entity createBackgroundCutscene(RenderSystem* renderer, vec2 position) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// scale the background
	motion.scale = vec2({ 1700, window_height_px });

	registry.cutscenes.emplace(entity);
	registry.backgrounds.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BGCUTSECNE ,
		 EFFECT_ASSET_ID::BATTLE,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;

	return entity;
}

Entity createMainMenu(RenderSystem* renderer, vec2 position) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// TODO: CHANGE MAIN MENU TEXTURE RENDERING HERE
	motion.scale = vec2({ -EAGLE_BB_WIDTH, EAGLE_BB_HEIGHT });
	registry.mainMenu.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ENEMYDRINK,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;
	rr.isStatic = true;
	
	return entity;
}

Entity createTutorialWindow(RenderSystem* renderer, vec2 position, int window) {
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = position - vec2(0, 150);
	motion.scale = vec2({ MENU_WIDTH * 2, MENU_HEIGHT * 2 + 100 }); // ??? i don't think this is right
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	if (window == 1) {
		registry.overWorld.emplace(entity);
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::TUTORIALBOARD,
			  EFFECT_ASSET_ID::BATTLE,
			  GEOMETRY_BUFFER_ID::SPRITE });

	}
	else if (window == 2) {
		registry.turnBased.emplace(entity);
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::BATTLEBOARD,
			  EFFECT_ASSET_ID::BATTLE,
			  GEOMETRY_BUFFER_ID::SPRITE });

	}

	registry.tutorials.emplace(entity);

	RenderRequest& rr = registry.renderRequests.get(entity);
	rr.isStatic = true;

	return entity;
}

Entity createLine(vec2 position, vec2 scale, float angle)
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::EGG,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE });

	// Create motion
	Motion& motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	registry.debugComponents.emplace(entity);
	return entity;
}

Entity create_chai(RenderSystem* renderer, vec2 pos) {

	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = mesh.original_size * 300.f;
	motion.scale.y *= -1; // point front to the right

	// Create and (empty) Chicken component to be able to refer to all eagles
	registry.players.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PLAYER, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::CHICKEN, // shuold prob fix this later
			GEOMETRY_BUFFER_ID::PLAYER });
	rr.shown = true;


	// give entity turn based components
	character_factory->construct_chai(entity);

	registry.colors.insert(entity, { 1, 0.8f, 0.8f });

	registry.turnBased.emplace(entity);

	return entity;
}
Entity create_americano(RenderSystem* renderer, vec2 pos) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = mesh.original_size * 300.f;
	motion.scale.y *= -1; // point front to the right


	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::AMERICANO, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::BATTLE, // shuold prob fix this later
			GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;

	// give entity turn based components
	character_factory->construct_americano(entity);

	registry.colors.insert(entity, { 1, 0.8f, 0.8f });

	registry.turnBased.emplace(entity);

	return entity;
}
Entity create_earl(RenderSystem* renderer, vec2 pos) {

	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = mesh.original_size * 300.f;
	motion.scale.y *= -1; // point front to the right


	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::EARL, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::BATTLE, // shuold prob fix this later
			GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;

	// give entity turn based components
	character_factory->construct_earl(entity);

	registry.colors.insert(entity, { 1, 0.8f, 0.8f });

	registry.turnBased.emplace(entity);

	return entity;
}

Entity create_london(RenderSystem* renderer, vec2 pos) {

	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = mesh.original_size * 300.f * 1.5f;
	motion.scale.y *= -1; // point front to the right


	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::LONDON, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::TEXTURED, // shuold prob fix this later
			GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;

	// give entity turn based components
	character_factory->construct_london(entity);

	registry.colors.insert(entity, { 1, 0.8f, 0.8f });
	registry.turnBased.emplace(entity);

	return entity;
}

Entity create_turn_based_enemy(RenderSystem* renderer, vec2 pos, int level) {

	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -EAGLE_BB_WIDTH, EAGLE_BB_HEIGHT });

	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ENEMYDRINK,
		 EFFECT_ASSET_ID::BATTLE,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;

	// give entity turn based components
	if (level == 0) character_factory->construct_tutorial_enemy(entity);
	else character_factory->construct_enemy(entity, level);
	
	
	registry.colors.insert(entity, { 1, 0.8f, 0.8f });

	registry.turnBased.emplace(entity);

	return entity;
}

Entity create_devil(RenderSystem* renderer, vec2 pos) {

	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	
	motion.velocity = { 0.f, 0.f };

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = mesh.original_size * 300.f * 1.5f;
	motion.scale.y *= -1; // point front to the right

	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::DEVIL,
		 EFFECT_ASSET_ID::BATTLE,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;

	// give entity turn based components
	character_factory->construct_ground_devil(entity);


	registry.colors.insert(entity, { 1, 0.8f, 0.8f });

	registry.turnBased.emplace(entity);

	return entity;
}

Entity createText(std::string text, vec2 position, float scale, vec3 color, glm::mat4 trans, StageSystem::Stage current_stage, bool isStatic) {
	auto entity = Entity();
	auto& textRenderRequest = registry.textRenderRequests.emplace(entity);
	textRenderRequest.text = text;
	textRenderRequest.position = position;
	textRenderRequest.scale = scale;
	textRenderRequest.color = color;
	textRenderRequest.trans = trans;

	// Adding text to specified stage
	switch (current_stage) {
	case StageSystem::Stage::main_menu: 
		registry.mainMenu.emplace(entity); 
		textRenderRequest.shown = true;
		break;
	case StageSystem::Stage::overworld:
		registry.overWorld.emplace(entity); 
		break;
	case StageSystem::Stage::cutscene:
		registry.cutscenes.emplace(entity);
		textRenderRequest.shown = true;
		break;
	case StageSystem::Stage::turn_based:
		registry.turnBased.emplace(entity); 
		break;
	case StageSystem::Stage::minigame:
		registry.miniStage.emplace(entity);
		break;
	}

	textRenderRequest.isStatic = isStatic;

	return entity;
}

Entity create_health_bar_outline(RenderSystem* renderer, vec2 pos) {
	
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = pos;

	// scale the background
	motion.scale = vec2({ HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT });

	// Create and (empty) Eagle component to be able to refer to all eagles
	registry.healthOutlines.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::HEALTHOUTLINE,
		 EFFECT_ASSET_ID::BATTLE,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;

	registry.turnBased.emplace(entity);

	return entity;
}

Entity create_health_bar_fill(RenderSystem* renderer, vec2 pos, Entity associated_character) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = pos;

	// scale the background
	motion.scale = vec2({ FILL_WIDTH, FILL_HEIGHT });

	// Create and (empty) Eagle component to be able to refer to all eagles
	HealthBarFill& fill = registry.healthBarFills.emplace(entity);
	fill.associated_char = associated_character;
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::HEALTHFILL,
		 EFFECT_ASSET_ID::BATTLEBAR,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;

	registry.turnBased.emplace(entity);

	return entity;
}


Entity create_cutscene_text_box(RenderSystem* renderer, int level, int selection, vec2 pos,  StageSystem::Stage current_stage) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = pos;

	// scale the background
	motion.scale = vec2({ TEXTBOX_WIDTH, TEXTBOX_HEIGHT });

	//add the text
	/*Entity text1 = createText( text, textPos,  scale,  color,  trans, current_stage);
	Entity text2 = createText(text2, { textPos.x, textPos.y-40}, scale, color, trans, current_stage);
	Entity text3 = createText(text3, { textPos.x, textPos.y -80 }, scale, color, trans, current_stage);
*/

	// Create and (empty) Eagle component to be able to refer to all eagles
	registry.cutscenes.emplace(entity);
	if (level == 0) {
		if (selection == 1) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::CUTSCENETEXTBOX1,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 2) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::CUTSCENETEXTBOX2,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 3) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::CUTSCENETEXTBOX3,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 4) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::CUTSCENETEXTBOX4,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
	}
	else if (level == 1) {
		if (selection == 1) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELONE1,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 2) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELONE2,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 3) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELONE3,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 4) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELONE4,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 5) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELONE5,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 6) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELONE6,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
	}
	else if (level == 2) {
		if (selection == 1) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTWO1,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 2) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTWO2,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 3) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTWO3,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 4) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTWO4,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 5) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTWO5,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 6) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTWO6,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 7) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTWO7,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
	}
	else if (level == 3) {
		if (selection == 1) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTHREE1,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 2) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTHREE2,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 3) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTHREE3,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 4) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTHREE4,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 5) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTHREE5,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 6) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTHREE6,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 7) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTHREE7,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 8) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTHREE8,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 9) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELTHREE9,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}

	}
	else if (level == 4) {
		if (selection == 1) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFOUR1,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 2) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFOUR2,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 3) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFOUR3,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 4) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFOUR4,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 5) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFOUR5,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 6) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFOUR6,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 7) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFOUR7,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 8) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFOUR8,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}

		}
	else if (level == 5) {
		if (selection == 1) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVE1,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 2) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVE2,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 3) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVE3,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 4) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVE4,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 5) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVE5,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 6) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVE6,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 7) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVE7,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 8) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVE8,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		if (selection == 9) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVE9,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection ==10) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVEREC1,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 11) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVEREC2,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 12) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVEREC3,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 13) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVEREC4,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 14) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVEREC5,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 15) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVEREC6,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 16) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVEREC7,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 17) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVEREC8,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 18) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::LEVELFIVEREC9,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
	}
	else if (level == 10) {
		if (selection == 1) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::ENDING1,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 2) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::ENDING2,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 3) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::ENDING3,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 4) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::ENDING4,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 5) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::ENDING5,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		else if (selection == 6) {
			RenderRequest& rq = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::ENDING6,
				 EFFECT_ASSET_ID::BATTLE,
				 GEOMETRY_BUFFER_ID::SPRITE });
			rq.shown = true;
		}
		
	}
	
	else {
	RenderRequest& rq = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::CUTSCENETEXTBOX3,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rq.shown = true;
	}
	
	return entity;
}

Entity create_sparkle(RenderSystem* renderer, vec2 pos, vec2 vel, vec2 acc, vec3 color) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = vel;
	motion.position = pos;

	// scale the sparkle FIX THIS TO BE SMALLER?
	motion.scale = vec2({ SPARKLE_LENGTH, SPARKLE_LENGTH });

	auto& sparkle = registry.sparkles.emplace(entity);
	sparkle.colour = color;
	sparkle.acceleration = acc;
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::SPARKLE,
		 EFFECT_ASSET_ID::SPARKLE,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;

	registry.turnBased.emplace(entity);
	return entity;
}