#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// These are ahrd coded to the dimensions of the entity texture
const float BUG_BB_WIDTH = 0.6f * 165.f;
const float BUG_BB_HEIGHT = 0.6f * 165.f;
const float EAGLE_BB_WIDTH = 0.6f * 300.f;
const float EAGLE_BB_HEIGHT = 0.6f * 202.f;
const float MENU_WIDTH = 0.6f * 700.f; 
const float MENU_HEIGHT = 0.6f * 280.f;
const float CUP_WIDTH = 0.6f * 700.f;
const float CUP_HEIGHT = 0.6f * 550.f;

// the player
Entity createChicken(RenderSystem* renderer, vec2 pos);
// Menu 
Entity createMenu(RenderSystem* renderer, vec2 pos);
// the prey
Entity createBug(RenderSystem* renderer, vec2 position);
// minigame
Entity createCup(RenderSystem* renderer, vec2 pos);
// minigame result 
Entity createMiniResult(RenderSystem* renderer, vec2 pos);
// the enemy
Entity createEagle(RenderSystem* renderer, vec2 position);
// the real enemy
Entity createEnemyDrink(RenderSystem* renderer, vec2 velocity, vec2 position);
// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);
// a egg
Entity createEgg(vec2 pos, vec2 size);


