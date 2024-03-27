#include "stage_system.hpp"

StageSystem::StageSystem()
{
	// stub;
}

void StageSystem::init()
{
	current_stage = main_menu;
	music_changed = false;
}

void StageSystem::set_stage(Stage target)
{
	current_stage = target;

	switch (current_stage) {
	case main_menu:
		set_main_menu();
		break;
	case overworld:
		set_overworld();
		break;
	case cutscene:
		set_cutscene();
		break;
	case turn_based:
		set_turn_based();
		break;
	case minigame:
		set_minigame();
		break;
	}

	music_changed = true;
}

void StageSystem::set_music_changed() {
	music_changed = !music_changed;
}

void StageSystem::set_render_shown(Entity entity, bool show_render, bool tutorial) {

	if (registry.renderRequests.has(entity)) {
		RenderRequest& render = registry.renderRequests.get(entity);
		if (tutorial) {
			if (!registry.tutorials.has(entity))
				render.shown = show_render;
		}
		else {
			render.shown = show_render;
		}
	}
	else if (registry.textRenderRequests.has(entity)) {
		TextRenderRequest& textRender = registry.textRenderRequests.get(entity);
		textRender.shown = show_render;
	}
}

void StageSystem::set_main_menu()
{
	// Set all main menu entities to be shown 
	for (Entity entity : registry.mainMenu.entities) {
		set_render_shown(entity, true, false);
	}

	// Set all overworld entities to not be shown
	for (Entity entity : registry.overWorld.entities) {
		set_render_shown(entity, false, false);
	}

	// Set all entites from turn_based to not be shown
	for (Entity entity : registry.turnBased.entities) {
		set_render_shown(entity, false, false);
	}

	// Set all entities from cutscene to not be shown
	for (Entity entity : registry.cutscenes.entities) {
		set_render_shown(entity, false, false);
	}

	// Set turn based entities to not be rendered
	for (Entity entity : registry.turnBased.entities) {
		set_render_shown(entity, false, false);
	}

	// Set all minigame based entities to not be shown
	for (Entity entity : registry.miniStage.entities) {
		set_render_shown(entity, false, false);
	}
}

void StageSystem::set_overworld()
{
	// Set all main menu entities to not be shown
	// Keeping entities live right now in case we need them
	for (Entity entity : registry.mainMenu.entities) {
		set_render_shown(entity, false, false);
	}

	// Set all overworld entities to be shown
	for (Entity entity : registry.overWorld.entities) {
		set_render_shown(entity, true, true);
	} 

	// Set all entites from turn_based to not be shown
	for (Entity entity : registry.turnBased.entities) {
		set_render_shown(entity, false, false);
	}
}

void StageSystem::set_cutscene()
{
	// Set all overworld entities to not be shown
	// Keep entities live since we still might need them
	for (Entity entity : registry.overWorld.entities) {
		set_render_shown(entity, false, false);
	}

	// Set all cutscene entities to be shown; 
	// this can be changed depending on how we want to decide which cutscenes are rendered
	for (Entity entity : registry.cutscenes.entities) {
		set_render_shown(entity, true, false);
	}
}

void StageSystem::set_turn_based()
{
	// Remove all components from cutscenes
	for (Entity entity : registry.cutscenes.entities) {
		registry.remove_all_components_of(entity);
	}

	// Set all minigame based entities to not be shown
	for (Entity entity : registry.miniStage.entities) {
		set_render_shown(entity, false, false);
	}

	// Set turn based entities to be rendered
	for (Entity entity : registry.turnBased.entities) {
		set_render_shown(entity, true, true);
	}
}

void StageSystem::set_minigame()
{
	// Set turn based entities to be rendered
	for (Entity entity : registry.turnBased.entities) {
		set_render_shown(entity, false, false);
	}

	// Set minigame based entities to be rendered 
	for (Entity entity : registry.miniStage.entities) {

	}
}