#include "cutscene_system_before.hpp"
#include "common.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

CutSceneSystemBefore::CutSceneSystemBefore() :
	cutscene_done(false),
	skip_cutscene(false)
{}

void CutSceneSystemBefore::init(StageSystem* stage_system_arg, RenderSystem* renderer) {
	stage_system = stage_system_arg;
	cutscene_done = false;
	skip_cutscene = false;
}

void CutSceneSystemBefore::handle_cutscene_render(RenderSystem* renderer) {
	// TODO: Change cutscene done to TRUE after some set amount of time
	//Entity textBox;

	if (cutscene_currently_rendered == 0) {
		createBackgroundCutscene(renderer, { window_width_px / 2.0, window_height_px / 2.0 });
	}


	if (cutscene_done) {
		// transition to overworld
		std::cout << "GO TO Overworld\n";
		skip_cutscene = false;
		cutscene_done = false;
		handle_cutscene();
		return;
	}

	if (this->cutscene_slide == cutscene_currently_rendered) {
		return;
	}

	Entity instructionsText = createText("Press left or right SHIFT to continue cutscene", { 10, 790 }, 1.2f, glm::vec3(1.0, 1.0, 1.0), glm::mat4(1.0f), StageSystem::Stage::cutscene, true);
	Entity instructionsText2 = createText("Press SPACE twice to skip cutscene", { 10, 740 }, 1.2f, glm::vec3(1.0, 1.0, 1.0), glm::mat4(1.0f), StageSystem::Stage::cutscene, true);
	switch (cutscene_slide) {
	case 1:

		textBox = create_cutscene_text_box(renderer, 0,1, vec2(750, 700), StageSystem::Stage::cutscene);
		//
		break;
	case 2:


		textBox = create_cutscene_text_box(renderer, 0,2, vec2(750, 700), StageSystem::Stage::cutscene);
		char1 = createCharPic(renderer, vec2(750, 500));
		// textBox = create_cutscene_text_box(renderer, vec2(0, 0));
		break;
	case 3:
		//registry.renderRequests.get(textBox).shown = false;
		textBox = create_cutscene_text_box(renderer, 0,3, vec2(750, 700), StageSystem::Stage::cutscene);

		char1 = createCharPic(renderer, vec2(750, 500));
		//textBox = create_cutscene_text_box(renderer, vec2(0, 0));
		break;
	case 4: 
		textBox = create_cutscene_text_box(renderer, 0,4, vec2(750, 700), StageSystem::Stage::cutscene);
		char1 = createCharPic(renderer, vec2(750, 500));
		break;

	}


	this->cutscene_currently_rendered = cutscene_slide;

}

Entity CutSceneSystemBefore::createCharPic(RenderSystem* renderer, vec2 pos)
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
	motion.scale = mesh.original_size * 700.f;
	motion.scale.y *= -1; // point front to the right

	// Create and (empty) Chicken component to be able to refer to all eagles
	registry.players.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PLAYER, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::CHICKEN, // shuold prob fix this later
			GEOMETRY_BUFFER_ID::PLAYER });
	rr.shown = true;


	registry.cutscenes.emplace(entity);

	return entity;
}

void CutSceneSystemBefore::handle_cutscene_keys(int key, int action) {
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_S) {
			if (cutscene_done) {
				// transition to turn-based
				std::cout << "GO TO overworld\n";
				skip_cutscene = false;
				cutscene_done = false;
				handle_cutscene();
				cutscene_slide = 1;
			}
		}
		if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {

			if (!cutscene_done) {
				// transition to turn-based
				if (char1) {
					registry.remove_all_components_of(char1);
				}
				if (char2) {
					registry.remove_all_components_of(char2);
				}
				if (textBox) {
					registry.remove_all_components_of(textBox);
				}
				cutscene_slide++;
				printf("You entered: %d", cutscene_slide);
			}
			if (cutscene_slide >= 5) {
				std::cout << "GO TO overworld\n";
				skip_cutscene = false;
				cutscene_done = false;
				handle_cutscene();
				cutscene_slide = 1;
			}
		}

		if (key == GLFW_KEY_SPACE) {
			if (!skip_cutscene) {
				std::cout << "SKIP CUTSCENE\n";
				skip_cutscene = true;
			}
			else if (skip_cutscene) {
				std::cout << "CUTSCENE DONE\n";
				cutscene_done = true;
			}
		}
	}
}

void CutSceneSystemBefore::handle_cutscene() {
	cutscene_currently_rendered = 0;


	stage_system->set_stage(StageSystem::Stage::overworld);
}

