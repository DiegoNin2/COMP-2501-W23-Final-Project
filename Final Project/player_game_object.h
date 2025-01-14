#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
#include <string>

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            //Getter
            inline int GetCollected(void) { return item_collected; }
            inline int GetPlayerHP(void) { return player_HP_;}
            inline bool IsInvincible(void) { return invincible; }
            inline int GetPlayerDamage(void) { return damage; }
            inline std::string GetWeaponType(void) { return weapon_type; }

            //Setter
            inline void SetPlayerHP(int hp) {player_HP_ = hp;}
            inline void SetItemCollected(int collected) { item_collected = collected; }
            inline void SetPlayerDamage(int dmg) { damage = dmg; }
            inline void SetWeaponType(std::string weapon) { weapon_type = weapon; }

            //invincibility power up
            void invincibility(GLuint exp_texture);

            //revert invincibility
            void revert(GLuint exp_texture);

        protected:
            //player hp
            int player_HP_;

            //damage 
            int damage;

            //item collected counter (incase we need it)
            int item_collected;

            //item timer (will be global for all timer powerups)
            float item_timer;
            float item_timer_end;

            //invincible state
            bool invincible;

            //gun type
            std::string weapon_type;

    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
