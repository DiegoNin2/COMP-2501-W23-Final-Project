#ifndef ENEMY_BULLET_GAME_OBJECT_H_
#define ENEMY_BULLET_GAME_OBJECT_H_

#include <vector>
#include "game_object.h"

namespace game {

    //i basically borrowed the assignment solution for this
    // Bullet game object
    class EnemyBulletGameObject : public GameObject {

    public:
        EnemyBulletGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

        // Update function for moving the player object around
        void Update(double delta_time) override;

        // Check for collisions with other game objects
        int Collision(const std::vector<GameObject*>& obj);

        //setter
        inline void SetContact(bool c) { contact = c; }

        //getter
        inline bool GotContact(void) { return contact; }

        glm::vec2 GetStart(void) { return start_; }

    protected:
        glm::vec2 start_; // Start position of the missle
        float current_time_; // Current time
        float last_time_; // Time at the last frame

        //bullet life timer
        float bullet_timer;
        float bullet_end_timer;

        //contact
        bool contact;

    }; // class BulletGameObject

} // namespace game

#endif // BULLET_GAME_OBJECT_H_
#pragma once
