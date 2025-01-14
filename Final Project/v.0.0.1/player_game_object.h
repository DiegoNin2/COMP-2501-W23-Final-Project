#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            //Getter
            inline int GetPlayerHP(void) { return player_HP_;}

            //Setter
            inline void SetPlayerHP(int hp) {player_HP_ = hp;}

        protected:
            //player hp
            int player_HP_;
    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
