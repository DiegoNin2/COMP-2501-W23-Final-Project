#include "item_game_object.h"

namespace game {

	/*
		ItemGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the item
	*/

	ItemGameObject::ItemGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, bool t, std::string i_type)
		: GameObject(position, geom, shader, texture) {
		type_ = Item;
		timed = t;
		item_taken = false;
		item_life = 0.0;
		item_life_end = 30.0;
		item_type = i_type;
	}

	void ItemGameObject::Collected() {
		item_taken = true;
	}

	// Update function for moving the item object around
	void ItemGameObject::Update(double delta_time) {

		// Special item updates go here
		item_life += delta_time;
		if (item_life >= item_life_end && !item_taken) {
			item_taken = true;
		}

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

} // namespace game
