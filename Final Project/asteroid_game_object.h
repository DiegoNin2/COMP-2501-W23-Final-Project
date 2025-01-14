#ifndef ASTEROID_GAME_OBJECT_H_
#define ASTEROID_GAME_OBJECT_H_

#include "game_object.h"


namespace game {

	//Inherits from GameObject

	enum Sizes{Big, Medium, Small};

	class AsteroidGameObject : public GameObject {

	public:

		AsteroidGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

		void Update(double delta_time) override;

		void ReduceSize();

		//getter
		inline Sizes GetSize(void) { return asteroid_size; }
		

	protected:

		//sizes
		Sizes asteroid_size;

	};

}

#endif