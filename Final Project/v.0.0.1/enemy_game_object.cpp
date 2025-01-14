#include "enemy_game_object.h"

namespace game {

/*
	EnemyGameObject inherits from GameObject
*/

	EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		type_ = Enemy;
		//patrolling = true;
	}

	void EnemyGameObject::Update(double delta_time) {

	// Special enemy updates
		/*// Check if object exploded
	if (state_ == Exploded) {
	// Increase explosion timer
		exp_time_ += delta_time;
		// Check if we reached the end of the explosion
		if (exp_time_ >= exp_end_time_) {
			// The object died now
			state_ = Died;
		}
	}*/

	// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}
}