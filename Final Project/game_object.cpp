#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "game_object.h"

namespace game {

GameObject::GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture) 
{

    // Initialize all attributes
    position_ = position;
    rotation_ = 0;
    angle_ = 0.0;
    scale_ = 1.0;
    velocity_ = glm::vec3(0.0f, 0.0f, 0.0f); // Starts out stationary
    geometry_ = geom;
    shader_ = shader;
    texture_ = texture;
    type_ = Other;
    state_ = Alive;
    tiling_ = 1.0;
}


glm::vec3 GameObject::GetBearing(void) {

    // Assumes sprite is initially rotated by 90 degrees
    float pi_over_two = glm::pi<float>() / 2.0f;
    glm::vec3 dir(cos(angle_ + pi_over_two), sin(angle_ + pi_over_two), 0.0);
    return dir;
}


glm::vec3 GameObject::GetRight(void) {

    // Assumes sprite is initially rotated by 90 degrees
    glm::vec3 dir(cos(angle_), sin(angle_), 0.0);
    return dir;
}


void GameObject::SetAngle(float angle) {

    // Set angle of the game object
    // Make sure angle is in the range [0, 2*pi]
    float two_pi = 2.0f * glm::pi<float>();
    angle = fmod(angle, two_pi);
    if (angle < 0.0) {
        angle += two_pi;
    }
    angle_ = angle;
}



void GameObject::Update(double delta_time) {

    // Update object position with Euler integration
    position_ += velocity_ * ((float) delta_time);

    //check state
    if (state_ == Exploded) {
        exp_time_ += delta_time;
        //check if timer ended
        if (exp_time_ >= exp_end_time_) {
            //object dead
            state_ = Dead;
        }
    }
}


void GameObject::Render(glm::mat4 view_matrix, double current_time){

    // Set up the shader
    shader_->Enable();

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_, 1.0));

    // Set up a rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0f, 0.0f, 1.0f));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    shader_->SetUniform1f("tiling", tiling_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

void GameObject::Explode(GLuint exp_texture) {
    //change the state
    state_ = Exploded;
    //start explosion timer & set end timer
    exp_time_ = 0.0;
    exp_end_time_ = 3.0;
    //change texture
    texture_ = exp_texture;
}

} // namespace game
