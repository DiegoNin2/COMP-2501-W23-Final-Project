#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.h"
#include "geometry.h"

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject {

        public:
            //types & states for objects
            enum State { Alive, Exploded, Dead };
            enum Type { Player, Enemy, Enemy2, Enemy3, Item, Weapon, EWeapon, Background, Particle, Turret, Shield, UI, Other };

            // Constructor
            GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Update the GameObject's state. Can be overriden in children
            virtual void Update(double delta_time);

            // Renders the GameObject 
            virtual void Render(glm::mat4 view_matrix, double current_time);

            // Getters
            inline glm::vec3& GetPosition(void) { return position_; }
            inline float GetRotation(void) {return rotation_; }
            inline float GetScale(void) { return scale_; }
            inline float GetAngle(void) { return angle_; }
            inline glm::vec3& GetVelocity(void) { return velocity_; }
            inline Type GetType(void) {return type_;}
            inline State GetState(void) {return state_;}

            inline float GetTiling() { return tiling_; }

            // Get bearing direction (direction in which the game object
            // is facing)
            glm::vec3 GetBearing(void);

            // Get vector pointing to the right side of the game objectd
            glm::vec3 GetRight(void);

            // Setters
            virtual void SetPosition( glm::vec3& position) {
                if (position[0] >= 10) { position[0] = 10.0; }
                if (position[0] <= -10) { position[0] = -10.0; }
                if (position[1] >= 10) { position[1] = 10.0; }
                if (position[1] <= -10) { position[1] = -10.0; }
                position_ = position; }
            inline void SetRotation(float angle){
                if (angle >6.28){angle -= 6.28;}
                if (angle <-6.28){angle += 6.28;}
                rotation_ = angle;
            }
            inline void SetScale(float scale) { scale_ = scale; }

            inline void SetVelocity(const glm::vec3& velocity) { velocity_ = velocity; }

            void SetAngle(float angle);

            inline void SetTiling(float t) { tiling_ = t; }

            void SetType(Type a) { type_ = a; }

            //other
            void Explode(GLuint exp_texture);

        protected:
            // Object's Transform Variables
            glm::vec3 position_;
            float rotation_;
            float scale_;
            float angle_;
            glm::vec3 velocity_;
            // TODO: Add more transformation variables

            //tiling factor
            float tiling_;

            // Geometry
            Geometry *geometry_;
 
            // Shader
            Shader *shader_;

            // Object's texture reference
            GLuint texture_;

            //object state
            State state_;

            //object type
            Type type_;

            //for direciton in enemy3
            int zig;
            float zig_timer;
            
            //explosion timers
            float exp_time_;
            float exp_end_time_;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
