#include "enemy_game_object3.h"
#include <iostream>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <string>
namespace game {

	/*
		EnemyGameObject inherits from GameObject
	*/

	EnemyGameObject3::EnemyGameObject3(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		type_ = Turret;
		velocity_ = glm::vec3(0.0f, 0.0f, 0.0f);

		turret_hp = 2;
		//patrolling = true;
	}

	void EnemyGameObject3::Update(double delta_time) {

		if (glm::length(position_ - target_->GetPosition()) < 4.5f) {
			shooting = true;
		}
		else {
			shooting = false;
		}

		//rotation
		float pi_over_two = glm::pi<float>() / 2.0f;
		glm::vec3 direction = glm::normalize(target_->GetPosition() - position_);
		angle_ = atan2(direction[1], direction[0]) - pi_over_two;

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}
}