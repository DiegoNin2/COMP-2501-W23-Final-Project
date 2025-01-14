#include "enemy_game_object.h"
#include <iostream>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <string>
namespace game {

/*
	EnemyGameObject inherits from GameObject
*/

	EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, int e)
		: GameObject(position, geom, shader, texture) {
		type_ = Enemy;
		if (e == 1) { type_ = Enemy; }
		if (e == 2) { type_ = Enemy3; scale_ = 0.7f; }
		velocity_ = glm::vec3(0.0f, 0.2f, 0.0f);
		state_ = Chasing;
		zig = -1;
		zig_timer = 0;
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

	// enemy3 code
	if (type_ == Enemy3) {
		//std::cout << "enemy type 3" << std::endl;
		glm::vec3 target_position = target_->GetPosition();
		//postion_
			float speed = 0.1f;
			float timer = 0.0f;
			//glm::vec3 steering = target_->GetPosition() - position_ - velocity_;
			glm::vec3 direction = glm::normalize(target_position - position_);// gives us unit vector of direction to player
			float pi_over_two = glm::pi<float>() / 2.0f;
			float num = 0.785;
			float dir = atan2f(direction[1], direction[0]);

			glm::vec3 ne(cos(dir + num / 2 + pi_over_two), sin(dir + num / 2 + pi_over_two), 0.0);
			glm::vec3 nw(cos(dir - num / 2 + pi_over_two), sin(dir - num / 2 + pi_over_two), 0.0);
			//velocity_ += (0.01f * ne);


			if (zig == 1) {
				velocity_ = (1.0f * ne * 1.0f);
				if (fmod(zig_timer, 2) < 0.01) {
					//std::cout << "zig: " << zig << std::endl;
					//std::cout << "zigtimer: " << zig_timer << std::endl;
					zig = 0;
					zig_timer = (fmod(zig_timer, 4));
				}
			}
			else if (zig == 0) {
				velocity_ = (1.0f * nw * 2.0f);
				if (fmod(zig_timer, 2) < 0.01) {
					//std::cout << "zig: " << zig << std::endl;
					//std::cout << "zigtimer: " << zig_timer << std::endl;
					zig = 1;
					zig_timer = (fmod(zig_timer, 4));
				}
			}
			else {
				velocity_ = (1.0f * nw);
				if (zig_timer > 1) {

					zig = 1;
					zig_timer - 1;
				}
			}
			zig_timer += delta_time;

	}
	// Check which state we are in
		if (state_ == Wandering) {
			// We coded this in class, but it's not quite there yet
			// Can you tweak it to make it work well?

			// Get angle of current velocity vector
			float angle = glm::atan(velocity_.y, velocity_.x);
			// Define the opening of the cone. I chose 30 degrees
			float th = (30.0 / 180.0) * glm::pi<float>();
			// Get random number between 0 and 1
			float a = (rand() % 10000) / 10000.0f;
			// Map random number in [0, 1] to [-1, 1]
			a = -1.0f + a * 2.0f;
			// Map [-1, 1] to [-th, th]
			a = a * th;
			// Set new angle around current angle
			float new_angle = angle + a;
			// Set velocity vector based on the new angle
			velocity_ = glm::vec3(cos(new_angle), sin(new_angle), 0.0);
			// If the player gets close enough, switch to the Chasing state
			float dist_threshold = 2.0f;
			if (glm::length(position_ - target_->GetPosition()) < dist_threshold) {
				state_ = Chasing;
			}
		}
		else if (state_ == Chasing) {
			// Linear motion to target
			/*
			// Target direction
			glm::vec3 td = target_->GetPosition() - position_;
			// Adjust velocity magnitude
			td = 1.0f * glm::normalize(td);
			// Set velocity as the adjusted target direction
			velocity_ = td;
			*/

			// Steering behavior: useful if the enemy is already moving
			// Yields a path that slowly steers towards the target
			//
			// Define steering force
			glm::vec3 steering = target_->GetPosition() - position_ - velocity_;
			// Uncomment for flee behavior
			//glm::vec3 steering = position_ - target_->GetPosition() - velocity_;
			// Adjust acceleration magnitude
			steering = 0.5f * glm::normalize(steering);
			// Add steering as acceleration to the velocity
			velocity_ += steering * ((float)delta_time);
		}

		//rotation
		float pi_over_two = glm::pi<float>() / 2.0f;
		glm::vec3 direction = glm::normalize(target_->GetPosition() - position_);
		angle_ = atan2(direction[1], direction[0]) - pi_over_two;

	// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}
}