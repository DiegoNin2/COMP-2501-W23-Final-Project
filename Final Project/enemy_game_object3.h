#ifndef ENEMY_GAME_OBJECT3_H_
#define ENEMY_GAME_OBJECT3_H_

#include "game_object.h"


namespace game {

	//Inherits from GameObject



	class EnemyGameObject3 : public GameObject {

	public:

		EnemyGameObject3(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

		void Update(double delta_time) override;
		// Update target
		GameObject* GetTarget(void) { return target_; }
		void SetTarget(GameObject* t) { target_ = t; }

		//getter
		inline bool CanShoot(void) { return shooting; }
		inline int GetTurretHP(void) { return turret_hp; }

		//setter
		inline void SetTurretHP(int hp) { turret_hp = hp; }

	protected:
		GameObject* target_;

		//able to shoot
		bool shooting;

		//turret hp
		int turret_hp;

	};

}

#endif