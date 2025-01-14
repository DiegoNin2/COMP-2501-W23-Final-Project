#ifndef DRAWING_GAME_OBJECT_H_
#define DRAWING_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class DrawingGameObject : public GameObject {

        public:
            DrawingGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Render function for the text
            void Render(glm::mat4 view_matrix, double current_time) override;

    }; // class DrawingGameObject

} // namespace game

#endif // DRAWING_GAME_OBJECT_H_
