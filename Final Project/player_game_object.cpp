#include "player_game_object.h"

namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture)
	: GameObject(position, geom, shader, texture) {
		type_ = Player;
		player_HP_ = 8;
		item_timer = 0.0;
		damage = 1;
		invincible = false;
		weapon_type = "gun";
	}

// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time) {

	// Special player updates go here

	//check if the player is invincible
	if (invincible) {
		//increase invincible timer
		item_timer += delta_time;
		//check if timer reached end
		if (item_timer >= item_timer_end) {
			//invincibility is off
			invincible = false;
		}
	}

	// Call the parent's update method to move the object in standard way, if desired
	GameObject::Update(delta_time);
}

void PlayerGameObject::invincibility(GLuint exp_texture) {

	//set invincible to true
	invincible = true;
	//start timer
	item_timer = 0.0;
	//end timer after 10 seconds
	item_timer_end = 10.0;
	//set texture
	texture_ = exp_texture;
}

void PlayerGameObject::revert(GLuint exp_texture) {
	//remove invincibility
	invincible = false;
	//set texture
	texture_ = exp_texture;
}

} // namespace game
