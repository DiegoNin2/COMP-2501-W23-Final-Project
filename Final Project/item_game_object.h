#ifndef ITEM_GAME_OBJECT_H_
#define ITEM_GAME_OBJECT_H_

#include "game_object.h"
#include <string>

namespace game {

    // Inherits from GameObject
    class ItemGameObject : public GameObject {

    public:
        ItemGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, bool timed, std::string type);

        // Update function for moving the player object around
        void Update(double delta_time) override;

        void Collected();

        //getter
        inline bool IsTimed(void) { return timed; }
        inline bool IsTaken(void) { return item_taken; }
        inline std::string GetItemType(void) { return item_type; }

    protected:
        //differentiate timed item or non timed
        bool timed;

        //item type
        std::string item_type;

        //item state
        bool item_taken;

        //life span for when they spawn
        double item_life;
        double item_life_end;

    }; // class ItemGameObject

} // namespace game

#endif // ITEM_GAME_OBJECT_H_
#pragma once
