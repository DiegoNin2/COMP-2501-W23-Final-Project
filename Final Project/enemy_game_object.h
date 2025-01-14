#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"


namespace game {

	//Inherits from GameObject

	enum EnemyState{Waiting, Wandering, Chasing, Tethering, Zooming};

	class EnemyGameObject : public GameObject {

		public:

			EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, int e);

			void Update(double delta_time) override;
			// Update target
			GameObject* GetTarget(void) { return target_; }
			void SetTarget(GameObject* t) { target_ = t; }

			void SetState(EnemyState s) { state_ = s; }

		protected:
			GameObject* target_;

			EnemyState state_;
	};

}

#endif