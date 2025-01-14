#include "bullet_game_object.h"
#include <iostream>

namespace game {

	BulletGameObject::BulletGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {

		// Store the missiles's start position
		start_ = glm::vec2(position.x, position.y);
		type_ = Weapon;

		// Initialize time
		current_time_ = 0.0;
		last_time_ = 0.0;
		bullet_timer = 0.0;
		bullet_end_timer = 2.0;

		contact = false;
	}


	void BulletGameObject::Update(double delta_time) {

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);

		// Update time variables
		last_time_ = current_time_;
		current_time_ += delta_time;

		//bullet KILL check
		bullet_timer += delta_time;
		//check if bullet life is over
		if (bullet_timer >= bullet_end_timer || contact) {
			//KILL BULLET
			state_ = Dead;
		}
		
	}

	// Generic function to return intersections between a ray and a circle
	//
	// Returns true if there are intersections, and false otherwise
	// The ray is defined as P + td, the circle has center C and radius r
	// The return values t1, t2 are the ray parameters of the intersection points
	// The values t1, t2 are undefined if there is no intersection
	bool RayCircleCollision(glm::vec2 P, glm::vec2 d, glm::vec2 C, float r, float& t1, float& t2) {

		// We derived these equations in class...
		glm::vec2 pc = P - C;
		float a = glm::dot(d, d);
		float b = 2.0f * glm::dot(d, pc);
		float c = glm::dot(pc, pc) - r * r;

		// Compute the roots of the quadratic equation ax^2 + bx + c = 0
		//
		// Compute the determinant first
		float delta = b * b - 4.0 * a * c;
		if (delta < 0.0f) {
			// If the determinant is negative, there are no intersections
			return false;
		}
		// Compute the roots of the equation
		t1 = (-b + sqrt(delta)) / (2 * a);
		t2 = (-b - sqrt(delta)) / (2 * a);
		// We have intersections
		return true;
	}

	// Check for collisions of the missile with all game objects given in obj
	int BulletGameObject::Collision(const std::vector<GameObject*>& obj) {

		// Convert velocity to a vec2
		glm::vec2 dir = glm::vec2(velocity_.x, velocity_.y);
		// Go through each object
		for (int i = 0; i < obj.size(); i++) {
			// Only check intersections with enemies
			if (obj[i]->GetType() == Enemy || obj[i]->GetType() == Enemy2 || obj[i]->GetType() == Enemy3 || obj[i]->GetType() == Turret) {
				// Get the enemy center and radius
				glm::vec3 pos = obj[i]->GetPosition();
				glm::vec2 C = glm::vec2(pos.x, pos.y);
				float r = obj[i]->GetScale();
				// Check for potential intersections
				float t1, t2;
				if (RayCircleCollision(start_, dir, C, r, t1, t2)) {
					// Check if the potential intersection t1 is a real intersection
					// If we interpret t1 as the time of intersection, we can check
					// if the time of the ray is between the last frame's time and 
					// the current time
					if ((t1 > last_time_) && (t1 <= current_time_)) {
						// Return index of object intersected
						return i;
						//std::cout << "Hit the object (t1)" << std::endl;
					}
					// Same test for t2
					if ((t2 > last_time_) && (t2 <= current_time_)) {
						return i;
						//std::cout << "Hit the object (t2)" << std::endl;
					}
				}
			}
		}
		return -1;
	}

} // namespace game
