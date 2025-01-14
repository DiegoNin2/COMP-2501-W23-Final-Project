#ifndef TEXT_GAME_OBJECT_H_
#define TEXT_GAME_OBJECT_H_

#include <string>

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class TextGameObject : public GameObject {

        public:
            TextGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GameObject *parent, std::string UI_Type);

            // Text to be displayed
            std::string GetText(void) const;
            void SetText(std::string text);

            //getter
            inline std::string GetUI(void) { return HUD; }

            // Render function for the text
            void Render(glm::mat4 view_matrix, double current_time) override;

        private:
            //text
            std::string text_;

            //parent
            GameObject* parent_;

            //text type
            std::string HUD;

    }; // class TextGameObject

} // namespace game

#endif // TEXT_GAME_OBJECT_H_
