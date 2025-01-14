#include "enemy_game_object2.h"
#include <iostream>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <string>

namespace game {

/*
	EnemyGameObject inherits from GameObject
*/

	EnemyGameObject2::EnemyGameObject2(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		type_ = Enemy2;
		velocity_ = glm::vec3(0.0f, 0.2f, 0.0f);
		//state_ = Chasing2;
		//patrolling = true;

		bool chasing = true;
		bool shooting = false;
		bool fleeing = false;
	}


	void EnemyGameObject2::Update(double delta_time) {
		// 3 or less = run | 5 or more is chase | 4 and between is shoot
		// Check which state we are in
		if (chasing) {
			//std::cout << "Chasing State" << std::endl;
			if (glm::length(position_ - target_->GetPosition()) < 3.0f) {
				//state_ = Fleeing2;
				fleeing = true;
				chasing = false;
			}
			else if (glm::length(position_ - target_->GetPosition()) < 4.5f) {
				//state_ = Shooting2;
				shooting = true;
				chasing = false;
			}
			else {
				glm::vec3 steering = target_->GetPosition() - position_ - velocity_;
				// Adjust acceleration magnitude
				steering = 0.5f * glm::normalize(steering);
				// Add steering as acceleration to the velocity
				velocity_ += steering * ((float)delta_time);

			}
		}

		if (shooting) {
			//std::cout << "Shooting State" << std::endl;
			if (glm::length(position_ - target_->GetPosition()) < 3.0f) {
				//std::cout << "changing to fleeing" << std::endl;
				//state_ = Fleeing2;
				shooting = false;
				fleeing = true;
			}
			else if (glm::length(position_ - target_->GetPosition()) > 5.0f) {
				//std::cout << "changing to chasing" << std::endl;
				//state_ = Chasing2;
				shooting = false;
				fleeing = true;
			}
			else {
				//std::cout << "in range, shooting" << std::endl;

				// Add steering as acceleration to the velocity
				velocity_ = glm::vec3(0.0f,0.0f,0.0f);
			}


		}
		if (fleeing) {
			//std::cout << "Fleeing State" << std::endl;
			if (glm::length(position_ - target_->GetPosition()) > 5.0f) {
				chasing = true;
				fleeing = false;
			}
			else if (glm::length(position_ - target_->GetPosition()) > 3.0f) {
				shooting = true;
				fleeing = false;
			}
			else {
				glm::vec3 steering = position_ - target_->GetPosition() - velocity_;
				// Adjust acceleration magnitude
				steering = 0.5f * glm::normalize(steering);
				// Add steering as acceleration to the velocity
				velocity_ += steering * ((float)delta_time);

			}
		}
		
		//rotation
		float pi_over_two = glm::pi<float>() / 2.0f;
		glm::vec3 direction = glm::normalize(target_->GetPosition() - position_);
		angle_ = atan2(direction[1], direction[0]) - pi_over_two;

	// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}
}