#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

	//Inherits from GameObject
	class EnemyGameObject : public GameObject {

		public:

			EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

			void Update(double delta_time);

	};

}

#endif