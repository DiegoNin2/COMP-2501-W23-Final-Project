#ifndef ENEMY_GAME_OBJECT2_H_
#define ENEMY_GAME_OBJECT2_H_

#include "game_object.h"


namespace game {

	//Inherits from GameObject

	enum EnemyState2{Chasing2, Shooting2, Fleeing2};

	class EnemyGameObject2 : public GameObject {

		public:

			EnemyGameObject2(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

			void Update(double delta_time) override;
			// Update target
			GameObject* GetTarget(void) { return target_; }
			void SetTarget(GameObject* t) { target_ = t; }
			void SetState(EnemyState2 s) { state_ = s; }

			//getters
			inline bool IsChasing(void) { return chasing; }
			inline bool IsShooting(void) { return shooting; }
			inline bool IsFleeing(void) { return fleeing; }

		protected:
			GameObject* target_;

			EnemyState2 state_;

			//states
			bool chasing;
			bool shooting;
			bool fleeing;
	};

}

#endif