#include "asteroid_game_object.h"
#include <iostream>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <string>
namespace game {

	/*
		AsteroidGameObject inherits from GameObject
	*/

	AsteroidGameObject::AsteroidGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		type_ = Asteroid;
		velocity_ = glm::vec3(0.0f, 0.2f, 0.0f);
		asteroid_size = Big;
		//patrolling = true;
	}

	void AsteroidGameObject::ReduceSize() {
		  
	}

	void AsteroidGameObject::Update(double delta_time) {

		//Special Asteroid updates

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}
}