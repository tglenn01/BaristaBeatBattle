#pragma once
#include "common.hpp"
#include <vector>
#include <unordered_map>
#include "../ext/stb_image/stb_image.h"
#include "turn_based_system/character_system.hpp"

struct MainMenu {

};

struct OverWorld {

};

struct CutScene {

};

struct TurnBased {

};

struct MiniStage {

};

struct TutorialBoard {

};

// This struct is just so I can have a nice curve in the motion of the player. 
// We add this component to the entity when it moves and then it helps process the animation. 

struct Animation {
	float current_ms = 0.f;
	float total_ms;
	vec2 start_pos;
	vec2 end_pos;
};

struct LevelNode
{
	vec2 position;
	int level_number;
	LevelNode* left_level;
	LevelNode* right_level;
	LevelNode* up_level;
	LevelNode* down_level;

};

// Player component
struct Player
{
	int level_num; // This is the level that the character is hovering at in the overworld, NOT the level of the character
};

// Eagles have a hard shell
struct Deadly
{

};

// Bug and Chicken have a soft shell
struct Eatable
{

};

// Scrolling background
struct Background
{

};

// Scrolling foreground
struct Foreground
{

};

// EnemyDrinks start fights if you talk/attack them
struct EnemyDrink
{
	Character* thisEnemy;
};

struct MenuOption
{
	bool active;
	std::string option;
};

struct Menu
{
	Entity options[3] = {};
	Entity associated_character;
	Entity activeOption;
};

struct Minigame
{

};

struct CharacterData {
	Character* characterData;
};

struct PartyMember {

};

struct TurnBasedEnemy {
	int experience_value = 0;
};

struct TurnCounter {
	int placement = 0;
	int speed_value = 0;
};

struct HealthOutline {
};

struct CutsceneSlideComp {

};

struct HealthBarFill {
	float percent_filled = 1.0f;
	Entity associated_char;
};

// All data relevant to the shape and motion of entities
struct Motion {
	vec2 position = { 0, 0 };
	float angle = 0;
	vec2 velocity = { 0, 0 };
	vec2 scale = { 10, 10 };
};

// Stucture to store collision information
struct Collision
{
	// Note, the first object is stored in the ECS container.entities
	Entity other; // the second object involved in the collision
	Collision(Entity& other) { this->other = other; };
};

// Data structure for toggling debug mode
struct Debug {
	bool in_debug_mode = 0;
	bool in_freeze_mode = 0;
};
extern Debug debugging;

// Sets the brightness of the screen
struct ScreenState
{
	float darken_screen_factor = -1;
};

// A struct to refer to debugging graphics in the ECS
struct DebugComponent
{
	// Note, an empty struct has size 1
};

// A timer that will be associated to dying chicken
struct DeathTimer
{
	float counter_ms = 3000;
};

// A timer associated with an attacking drink
struct AttackTimer
{
	float counter_ms = 700; // might change this number
};

enum class minigame_state {
	normal,
	perfect,
	good,
	fail
};

struct InjuredTimer
{
	float counter_ms = 3000;
	float redness_factor = 1.0f;
};

struct MiniGameTimer
{
	float counter_ms = 12000;
	float inter_timer = 500;
	minigame_state cup_state = minigame_state::normal;
};

struct MiniGameResTimer
{
	float counter_ms = 250;
	minigame_state res_state = minigame_state::normal;
};

struct MiniGameVisualizer
{
	minigame_state res_state = minigame_state::good;
};

struct PersistenceFeedbackTimer
{
	float counter_ms = 1000;
};


// Single Vertex Buffer element for non-textured meshes (coloured.vs.glsl & chicken.vs.glsl)
struct ColoredVertex
{
	vec3 position;
	vec3 color;
};

// Single Vertex Buffer element for textured sprites (textured.vs.glsl)
struct TexturedVertex
{
	vec3 position;
	vec2 texcoord;
};

// Mesh datastructure for storing vertex and index buffers
struct Mesh
{
	static bool loadFromOBJFile(std::string obj_path, std::vector<ColoredVertex>& out_vertices, std::vector<uint16_t>& out_vertex_indices, vec2& out_size);
	vec2 original_size = {1,1};
	std::vector<ColoredVertex> vertices;
	std::vector<uint16_t> vertex_indices;
};

/**
 * The following enumerators represent global identifiers refering to graphic
 * assets. For example TEXTURE_ASSET_ID are the identifiers of each texture
 * currently supported by the system.
 *
 * So, instead of referring to a game asset directly, the game logic just
 * uses these enumerators and the RenderRequest struct to inform the renderer
 * how to structure the next draw command.
 *
 * There are 2 reasons for this:
 *
 * First, game assets such as textures and meshes are large and should not be
 * copied around as this wastes memory and runtime. Thus separating the data
 * from its representation makes the system faster.
 *
 * Second, it is good practice to decouple the game logic from the render logic.
 * Imagine, for example, changing from OpenGL to Vulkan, if the game logic
 * depends on OpenGL semantics it will be much harder to do the switch than if
 * the renderer encapsulates all asset data and the game logic is agnostic to it.
 *
 * The final value in each enumeration is both a way to keep track of how many
 * enums there are, and as a default value to represent uninitialized fields.
 */

enum class TEXTURE_ASSET_ID {
	BUG = 0,
	EAGLE = BUG + 1,
	ENEMYDRINK = EAGLE + 1,

	BATTLEBOARD = ENEMYDRINK + 1,
	TUTORIALBOARD = BATTLEBOARD + 1,
	GAMEOVERBOARD = TUTORIALBOARD + 1,
	ATTACKBUTTON = GAMEOVERBOARD + 1,
	ITEMBUTTON = ATTACKBUTTON + 1,
	RESTBUTTON = ITEMBUTTON + 1,
	MINIGAMECOOLPERFECT = RESTBUTTON + 1,
	MINIGAMECOOLGOOD = MINIGAMECOOLPERFECT + 1,
	MINIGAMECOOLCLOUD = MINIGAMECOOLGOOD + 1,
	MINIGAMECUP = MINIGAMECOOLCLOUD + 1,
	MINIGAMECUPGOOD = MINIGAMECUP + 1,
	MINIGAMECUPBAD = MINIGAMECUPGOOD + 1,
	MINIGAMEPERFECT = MINIGAMECUPBAD + 1,
	MINIGAMEGOOD = MINIGAMEPERFECT + 1,
	MINIGAMEFAIL = MINIGAMEGOOD + 1,
	BGSCROLL = MINIGAMEFAIL + 1,
	FGSCROLL = BGSCROLL + 1,
	FGLIGHT = FGSCROLL + 1,
	BGBATTLE = FGLIGHT + 1,
	PLAYER = BGBATTLE + 1,
	HEALTHOUTLINE = PLAYER + 1,
	HEALTHFILL = HEALTHOUTLINE + 1,
	BGCUTSECNE = HEALTHFILL + 1,
	CUTSCENETEXTBOX1 = BGCUTSECNE + 1,
	CUTSCENETEXTBOX2 = CUTSCENETEXTBOX1 + 1,
	CUTSCENETEXTBOX3 = CUTSCENETEXTBOX2 + 1,
	EARL = CUTSCENETEXTBOX3 + 1,
	AMERICANO = EARL + 1,
	LEVEL1 = AMERICANO + 1,
	LEVEL2 = LEVEL1 + 1,
	LEVEL3 = LEVEL2 + 1,
	LEVEL4 = LEVEL3 + 1,
	LEVEL5 = LEVEL4 + 1,
	TEXTURE_COUNT = LEVEL5 + 1,
};
const int texture_count = (int)TEXTURE_ASSET_ID::TEXTURE_COUNT;

enum class EFFECT_ASSET_ID {
	COLOURED = 0,
	EGG = COLOURED + 1,
	CHICKEN = EGG + 1,
	TEXTURED = CHICKEN + 1,
	WIND = TEXTURED + 1,
	BACKGROUND = WIND + 1,
	FOREGROUND = BACKGROUND + 1,
	LIGHTS = FOREGROUND + 1,
	BATTLE = LIGHTS + 1,
	BATTLEBAR = BATTLE + 1,
	EFFECT_COUNT = BATTLEBAR + 1
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID {
	CHICKEN = 0,
	SPRITE = CHICKEN + 1,
	EGG = SPRITE + 1,
	DEBUG_LINE = EGG + 1,
	SCREEN_TRIANGLE = DEBUG_LINE + 1,
	PLAYER = SCREEN_TRIANGLE + 1,
	GEOMETRY_COUNT = PLAYER + 1
};
const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

struct RenderRequest {
	TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
	GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
	bool shown = false;
	bool isStatic = false;
};


struct TextRenderRequest {
	std::string text;
	vec2 position = { 0,0 };
	float scale;
	vec3 color;
	glm::mat4 trans;
	bool shown = false;
	bool isStatic = false;
};

