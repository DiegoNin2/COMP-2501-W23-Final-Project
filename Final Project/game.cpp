#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>
#include <time.h>
#include <fstream>

#include <path_config.h>

#include "sprite.h"
#include "shader.h"
#include "text_game_object.h"
#include "drawing_game_object.h"
#include "player_game_object.h"
#include "game.h"
#include "enemy_game_object.h"
#include "enemy_game_object2.h"
#include "enemy_game_object3.h"
#include "item_game_object.h"
#include "bullet_game_object.h"
#include "particles.h"
#include "particle_system.h"
#include "text_game_object.h"
#include "enemy_bullet_game_object.h"


namespace game {

    // Some configuration constants
    // They are written here as global variables, but ideally they should be loaded from a configuration file

    // Globals that define the OpenGL window and viewport
    const char* window_title_g = "v0.0.1";
    const unsigned int window_width_g = 800;
    const unsigned int window_height_g = 600;
    const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

    // Directory with game resources such as textures
    const std::string resources_directory_g = RESOURCES_DIRECTORY;

//items
const std::string item_1 = "HP";
const std::string item_2 = "DMGUP";
const std::string item_3 = "INVINCIBLE";


Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


    void Game::Init(void)
    {

        srand(time(NULL));

        // Initialize the window management library (GLFW)
        if (!glfwInit()) {
            throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
        }

        // Set window to not resizable
        // Required or else the calculation to get cursor pos to screenspace will be incorrect
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // Create a window and its OpenGL context
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
        if (!window_) {
            glfwTerminate();
            throw(std::runtime_error(std::string("Could not create window")));
        }

        // Make the window's OpenGL context the current one
        glfwMakeContextCurrent(window_);

        // Initialize the GLEW library to access OpenGL extensions
        // Need to do it after initializing an OpenGL context
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char*)glewGetErrorString(err))));
        }

        // Set event callbacks
        glfwSetFramebufferSizeCallback(window_, ResizeCallback);

        // Initialize sprite geometry
        sprite_ = new Sprite();
        sprite_->CreateGeometry();

    // Initialize particle geometry
    particles_ = new Particles();
    particles_->CreateGeometry();

    explosion_particles_ = new Particles();
    explosion_particles_->CreateExplosionGeometry();

    // Initialize particle shader
    particle_shader_.Init((resources_directory_g + std::string("/particle_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/particle_fragment_shader.glsl")).c_str());

    // Initialize sprite shader
    sprite_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/sprite_fragment_shader.glsl")).c_str());

    // Initialize text shader
    text_shader_.Init((resources_directory_g + std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g + std::string("/text_fragment_shader.glsl")).c_str());

    // Initialize time
    current_time_ = 0.0;
    bullet_cooldown = 0.0;
    enemy_spawn_time_end = 7.0;
    item_spawn_time_end = 20.0;
    enemy_spawn_timer = 0.0;
    item_spawn_timer = 0.0;
    shotgun_cooldown = 0.0;
    enemy_bullet_cooldown = 0.0;
    score = 0;
}


Game::~Game()
{
    // Free memory for all objects
    // Only need to delete objects that are not automatically freed
    delete sprite_;
    delete particles_;
    for (int i = 0; i < game_objects_.size(); i++){
        delete game_objects_[i];
    }

        // Close window
        glfwDestroyWindow(window_);
        glfwTerminate();
    }


void Game::Setup(void)
    {

        // Setup the game world

        // Load textures
        SetAllTextures();

        // Setup the player object (position, texture, vertex count)
        // Note that, in this specific implementation, the player object should always be the first object in the game object vector 
        game_objects_.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[0]));

    game_objects_.push_back(new ItemGameObject(glm::vec3(2.0f, 2.0f, 0.0f), sprite_, &sprite_shader_, tex_[5], false, item_1));
    game_objects_[1]->SetScale(0.5);

    //the only turret enemies that exist in the game
    EnemyGameObject3* enemy3 = new EnemyGameObject3(glm::vec3(5.0f, 5.0f, 0.0f), sprite_, &sprite_shader_, tex_[1]);
    enemy3->SetTarget(game_objects_[0]);
    game_objects_.push_back(enemy3);

    EnemyGameObject3* enemy4 = new EnemyGameObject3(glm::vec3(-5.0f, -5.0f, 0.0f), sprite_, &sprite_shader_, tex_[1]);
    enemy4->SetTarget(game_objects_[0]);
    game_objects_.push_back(enemy4);

    EnemyGameObject3* enemy12 = new EnemyGameObject3(glm::vec3(5.0f, -5.0f, 0.0f), sprite_, &sprite_shader_, tex_[1]);
    enemy12->SetTarget(game_objects_[0]);
    game_objects_.push_back(enemy12);

    EnemyGameObject3* enemy21 = new EnemyGameObject3(glm::vec3(-5.0f, 5.0f, 0.0f), sprite_, &sprite_shader_, tex_[1]);
    enemy21->SetTarget(game_objects_[0]);
    game_objects_.push_back(enemy21);

    // Set up text quad
    PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(game_objects_[0]);
    TextGameObject* text = new TextGameObject(glm::vec3(-2.8f, 3.5f, 0.0f), sprite_, &text_shader_, tex_[10], game_objects_[0], "HP");
    text->SetScale(2.0);
    text->SetText("Player HP: " + std::to_string(player->GetPlayerHP()));
    game_objects_.push_back(text);

    TextGameObject* text2 = new TextGameObject(glm::vec3(-2.8f, 3.0f, 0.0f), sprite_, &text_shader_, tex_[10], game_objects_[0], "Time");
    text2->SetScale(2.0);
    text2->SetText("Time: ");
    game_objects_.push_back(text2);

    TextGameObject* text3 = new TextGameObject(glm::vec3(-2.8f, 2.5f, 0.0f), sprite_, &text_shader_, tex_[10], game_objects_[0], "Weapon");
    text3->SetScale(2.0);
    text3->SetText("Weapon: " + player->GetWeaponType());
    game_objects_.push_back(text3);

    TextGameObject* text4 = new TextGameObject(glm::vec3(2.8f, 3.5f, 0.0f), sprite_, &text_shader_, tex_[10], game_objects_[0], "Score");
    text3->SetScale(2.0);
    text3->SetText("Score: ");
    game_objects_.push_back(text4);

    //game_objects_.push_back(new EnemyGameObject(glm::vec3(-1.0f, 1.0f, 0.0f), sprite_, &sprite_shader_, tex_[1]));
    //game_objects_.push_back(new EnemyGameObject(glm::vec3(1.0f, -0.5f, 0.0f), sprite_, &sprite_shader_, tex_[2]));
    //game_objects_.push_back(new EnemyGameObject(glm::vec3(2.0f, -0.5f, 0.0f), sprite_, &sprite_shader_, tex_[2]));

    // Setup background
    // In this specific implementation, the background is always the
    // last object
    GameObject *background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
    background->SetScale(100.0);
    background->SetTiling(10.0);
    game_objects_.push_back(background);

    // Setup particle system
    GameObject* particles = new ParticleSystem(glm::vec3(0.0f, -0.5f, 0.0f), particles_, &particle_shader_, tex_[9], game_objects_[0]);
    particles->SetScale(0.2);
    game_objects_.push_back(particles);
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
    {

        // Set OpenGL viewport based on framebuffer width and height
        glViewport(0, 0, width, height);
    }


void Game::SetTexture(GLuint w, const char* fname)
    {
        // Bind texture buffer
        glBindTexture(GL_TEXTURE_2D, w);

        // Load texture from a file to the buffer
        int width, height;
        unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);

        // Texture Wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Texture Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }


void Game::SetAllTextures(void)
{
    // Load all textures that we will need
    glGenTextures(NUM_TEXTURES, tex_);
    SetTexture(tex_[0], (resources_directory_g+std::string("/textures/destroyer_red.png")).c_str());
    SetTexture(tex_[1], (resources_directory_g+std::string("/textures/destroyer_green.png")).c_str());
    SetTexture(tex_[2], (resources_directory_g+std::string("/textures/destroyer_blue.png")).c_str());
    SetTexture(tex_[3], (resources_directory_g+std::string("/textures/stars.png")).c_str());
    SetTexture(tex_[4], (resources_directory_g+std::string("/textures/explosion.png")).c_str());
    SetTexture(tex_[5], (resources_directory_g + std::string("/textures/heart.png")).c_str());
    SetTexture(tex_[6], (resources_directory_g + std::string("/textures/sword.png")).c_str());
    SetTexture(tex_[7], (resources_directory_g + std::string("/textures/shield.png")).c_str());
    SetTexture(tex_[8], (resources_directory_g + std::string("/textures/bullet.png")).c_str());
    SetTexture(tex_[9], (resources_directory_g + std::string("/textures/orb.png")).c_str());
    SetTexture(tex_[10], (resources_directory_g + std::string("/textures/font.png")).c_str());
    SetTexture(tex_[11], (resources_directory_g + std::string("/textures/invincible.png")).c_str());
    SetTexture(tex_[12], (resources_directory_g + std::string("/textures/enemy_ship1.png")).c_str());
    SetTexture(tex_[13], (resources_directory_g + std::string("/textures/enemy_ship2.png")).c_str());
    SetTexture(tex_[14], (resources_directory_g + std::string("/textures/enemy_bullet.png")).c_str());
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}


void Game::MainLoop(void)
    {
        // Loop while the user did not close the window
        double last_time = glfwGetTime();
        while (!glfwWindowShouldClose(window_)) {

            // Clear background
            glClearColor(viewport_background_color_g.r,
                viewport_background_color_g.g,
                viewport_background_color_g.b, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Set view to zoom out, centered by default at 0,0
            float camera_zoom = 0.25f;
            glm::mat4 camera_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom));
            glm::mat4 camera_translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-game_objects_[0]->GetPosition()[0], -game_objects_[0]->GetPosition()[1], 0));
            glm::mat4 view_matrix = camera_scale_matrix * camera_translation_matrix;

            // Calculate delta time
            double current_time = glfwGetTime();
            double delta_time = current_time - last_time;
            last_time = current_time;

            // Update other events like input handling
            glfwPollEvents();

        // spawn items/enemies
        item_spawn_timer += delta_time;
        if (item_spawn_timer >= item_spawn_time_end) {
            //std::cout << "item spawning..." << std::endl;
            SpawnItems();
            item_spawn_timer = 0.0;
        }
        enemy_spawn_timer += delta_time;
        if (enemy_spawn_timer >= enemy_spawn_time_end) {
            SpawnEnemies();
            enemy_spawn_timer = 0.0;
        }

        // Update the game
        Update(view_matrix, delta_time);

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);
    }
}

void Game::SpawnItems() {
    float x, y;
    
    //ranges
    int minX = game_objects_[0]->GetPosition().x - 4.5;
    int maxX = game_objects_[0]->GetPosition().x + 4.5;
    int minY = game_objects_[0]->GetPosition().y - 4.5;
    int maxY = game_objects_[0]->GetPosition().y + 4.5;

    //generate random pos
    srand(time(NULL));
    x = rand() % (maxX - minX + 1) + minX;
    y = rand() % (maxY - minY + 1) + minY;

    //get distance between new pos and player pos
    float dist = sqrt(pow(x - game_objects_[0]->GetPosition().x, 2) + pow(y - game_objects_[0]->GetPosition().y, 2));

    //check if it is too close to the player
    if (dist < 2.0f) {
        //get a new one if so
        std::cout << "location too close, retrying..." << std::endl;
        SpawnItems();
        return;
    }

    //generate a random item
    srand(time(NULL));
    int rand_item = 1 + (rand() % 3);

    //check the val for the coresponding item
    if (rand_item == 1) { 
        //HP item
        ItemGameObject* item = new ItemGameObject(glm::vec3(x, y, 0.0f), sprite_, &sprite_shader_, tex_[5], false, item_1);
        item->SetScale(0.5);
        game_objects_.insert(game_objects_.begin() + 1, item);
        std::cout << "spawned hp at: "<< x << " " << y << std::endl;
    }
    else if (rand_item == 2) {
        //DMG up item
        ItemGameObject* item = new ItemGameObject(glm::vec3(x, y, 0.0f), sprite_, &sprite_shader_, tex_[6], false, item_2);
        item->SetScale(0.5);
        game_objects_.insert(game_objects_.begin() + 1, item);
        std::cout << "spawned dmg at: " << x << " " << y << std::endl;
    }
    else {
        //Invincible item
       ItemGameObject* item = new ItemGameObject(glm::vec3(x, y, 0.0f), sprite_, &sprite_shader_, tex_[7], false, item_3);
       item->SetScale(0.5);
       game_objects_.insert(game_objects_.begin() + 1, item);
       std::cout << "spawned invinc at: " << x << " " << y << std::endl;
    }
}

void Game::SpawnEnemies() {
        //spawn new enemy
        std::cout << "Spawning Enemy " << std::endl;
        float randomX = ((float)rand()) / (float)RAND_MAX;
        float randomY = ((float)rand()) / (float)RAND_MAX;
        randomX -= 0.5;
        randomY -= 0.5;
        if (randomX < 0) {
            randomX = int(game_objects_[0]->GetPosition()[0]) - 4.5;
        }
        else {
            randomX = int(game_objects_[0]->GetPosition()[0]) + 4.5;
        }
        if (randomY < 0) {
            randomY = int(game_objects_[0]->GetPosition()[1] - 4.5);
        }
        else {
            randomY = int(game_objects_[0]->GetPosition()[1]) + 4.5;
        }

        // random num between 1 and 3 for which enemy to spawn
        int rng = rand() % 3;
        if (rng == 0) {
            // spawn enemy type 1 (runs at u)
            EnemyGameObject* enemy = new EnemyGameObject(glm::vec3((float)randomX, (float)randomY, 0.0f), sprite_, &sprite_shader_, tex_[13], 1);
            enemy->SetTarget(game_objects_[0]);
            enemy->SetState(Chasing);
            game_objects_.insert(game_objects_.begin() + 2, enemy);
        }
        else if (rng == 1) {
            // spawns enemy type 2 (shoots u)
            EnemyGameObject2* enemy2 = new EnemyGameObject2(glm::vec3((float)randomX, (float)randomY, 0.0f), sprite_, &sprite_shader_, tex_[12]);
            enemy2->SetTarget(game_objects_[0]);
            enemy2->SetState(Chasing2);
            game_objects_.insert(game_objects_.begin() + 2, enemy2);
        }
        else if (rng == 2) {
            //spawns enemy type 3 (runs at u weirdly and is small)
            EnemyGameObject* enemy3 = new EnemyGameObject(glm::vec3((float)randomX, (float)randomY, 0.0f), sprite_, &sprite_shader_, tex_[2], 2);
            enemy3->SetTarget(game_objects_[0]);
            enemy3->SetState(Chasing);
            game_objects_.insert(game_objects_.begin() + 2, enemy3);
        }
}

void Game::Update(glm::mat4 view_matrix, double delta_time)
{

        // Update time
        current_time_ += delta_time;

        // Handle user input
        Controls(delta_time);

        // Update and render all game objects
        for (int i = 0; i < game_objects_.size(); i++) {
            // Get the current game object
            GameObject* current_game_object = game_objects_[i];
            //std::cout << "current game object type: " << current_game_object->GetType()<<std::endl;
            // Update the current game object
            current_game_object->Update(delta_time);
            

            if (current_game_object->GetType() == GameObject::Enemy2) {
                // if ranged enemy, and shooting state, shoot
                EnemyGameObject2* enemy = dynamic_cast<EnemyGameObject2*>(current_game_object);

                if (enemy->IsShooting() && enemy->GetState() == GameObject::Alive) {
                    //std::cout << "FIRING BULLET" << std::endl;
                    if (current_time_ > enemy_bullet_cooldown) {
                        glm::vec3 direction = glm::normalize(game_objects_[0]->GetPosition() - current_game_object->GetPosition());
                        float dir = atan2f(direction[1], direction[0]);

                        // Create and initialize the missile
                        EnemyBulletGameObject* missile = new EnemyBulletGameObject(current_game_object->GetPosition(), sprite_, &sprite_shader_, tex_[14]);
                        missile->SetVelocity(2.5f * -game_objects_[0]->GetBearing());
                        missile->SetScale(0.3f);

                        // Set the orientation of the bullet with the player's orientation
                        missile->SetAngle(dir);

                        // Add missile to list of game objects
                        // We should add it before the other unrelated game objectsdddd
                        game_objects_.insert(game_objects_.begin() + 3, missile);

                        // Set next allowed firing time
                        enemy_bullet_cooldown = current_time_ + 2.0f;
                    }
                }
            }
            else if (current_game_object->GetType() == GameObject::Turret) {
                //if turret found and is in range, shoot
                EnemyGameObject3* enemy = dynamic_cast<EnemyGameObject3*>(current_game_object);
                if (enemy->CanShoot() && enemy->GetState() == GameObject::Alive) {
                    //std::cout << "FIRING BULLET" << std::endl;
                    if (current_time_ > enemy_bullet_cooldown) {
                        glm::vec3 direction = glm::normalize(game_objects_[0]->GetPosition() - current_game_object->GetPosition());
                        float dir = atan2f(direction[1], direction[0]);

                        // Create and initialize the missile
                        EnemyBulletGameObject* missile = new EnemyBulletGameObject(current_game_object->GetPosition(), sprite_, &sprite_shader_, tex_[14]);
                        missile->SetVelocity(2.5f * -game_objects_[0]->GetBearing());
                        missile->SetScale(0.3f);

                        // Set the orientation of the bullet with the player's orientation
                        missile->SetAngle(dir);

                        // Add missile to list of game objects
                        // We should add it before the other unrelated game objectsdddd
                        game_objects_.insert(game_objects_.begin() + 3, missile);

                        // Set next allowed firing time
                        enemy_bullet_cooldown = current_time_ + 1.0f;
                    }
                }
            }

        //check if the invincibility is off
        if (current_game_object->GetType() == GameObject::Player) {
            PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(current_game_object);
            if (!player->IsInvincible()) {
                player->revert(tex_[0]);
            }
        }

        //KILL check
        if (current_game_object->GetState() == GameObject::Dead 
            && (current_game_object->GetType() == GameObject::Enemy || current_game_object->GetType() == GameObject::Enemy3)) {
            //delete enemy from the universe
            game_objects_.erase(game_objects_.begin() + i);
            //delete memory and continue with loop
            delete current_game_object;
            i--;
            continue;
        }
        else if (current_game_object->GetState() == GameObject::Dead 
            && (current_game_object->GetType() == GameObject::Enemy2 || current_game_object->GetType() == GameObject::Turret)) {
            //delete enemy from the universe
            game_objects_.erase(game_objects_.begin() + i);
            //delete memory and continue with loop
            delete current_game_object;
            i--;
            continue;
        }

        //player KILL check
        if (current_game_object->GetType() == GameObject::Player && current_game_object->GetState() == GameObject::Dead) {
             std::cout << "Game Over" << std::endl;
             std::cout << score << std::endl;
             glfwSetWindowShouldClose(window_, true);
        }

            //item KILL check
            if (current_game_object->GetType() == GameObject::Item) {
                ItemGameObject* item = dynamic_cast<ItemGameObject*>(current_game_object);
                if (item->IsTaken()) {
                    //delete from memory
                    game_objects_.erase(game_objects_.begin() + i);
                    delete current_game_object;
                    //advance the loop
                    i--;
                    continue;
                }
            }

            //bullet KILL check
            if (current_game_object->GetType() == GameObject::Weapon && current_game_object->GetState() == GameObject::Dead) {
                //delete bullet from this multiverse
                game_objects_.erase(game_objects_.begin() + i);
                //delete memory and continue with loop
                delete current_game_object;
                i--;
                std::cout << "bullet died" << std::endl;
                continue;
            }
            else if (current_game_object->GetType() == GameObject::EWeapon && current_game_object->GetState() == GameObject::Dead) {
                //delete bullet from this world
                game_objects_.erase(game_objects_.begin() + i);
                //delete memory and continue with loop
                delete current_game_object;
                i--;
                std::cout << "bullet died" << std::endl;
                continue;
            }

        //explosion particle KILL check
        if (current_game_object->GetState() == GameObject::Dead && current_game_object->GetType() == GameObject::Particle) {
            // Remove object from the list of objects
            game_objects_.erase(game_objects_.begin() + i);
            // Delete game object from memory
            delete current_game_object;
            // You could also keep the object and respawn it later...
            // Make sure we don't skip an element when we advance the loop
            i--;
            std::cout << "particle gone" << std::endl;
            continue;
        }

        // Check for collision with other game objects
        // Note the loop bounds: we avoid testing the last object since
        // it's the background covering the whole game world
        for (int j = i + 1; j < (game_objects_.size()-1); j++) {
            GameObject* other_game_object = game_objects_[j];

                // Compute distance between object i and object j
                float distance = glm::length(current_game_object->GetPosition() - other_game_object->GetPosition());
                // If distance is below a threshold, we have a collision
                if (distance < 0.8f) {
                    // This is where you would perform collision response between objects

                //check game object type
                if (other_game_object->GetType() == GameObject::Enemy && other_game_object->GetState() == GameObject::Alive) {
                    //insert enemy object dynamic cast here
                    EnemyGameObject* enemy = dynamic_cast<EnemyGameObject*>(other_game_object);
                    //handle player collision (if its the player)
                    if (current_game_object->GetType() == GameObject::Player && enemy->GetState() == GameObject::Alive) {
                        //KILL ENEMY & increase score
                        enemy->Explode(tex_[4]);
                        score++;

                        //create explosion particle system here
                        GameObject* particles = new ParticleSystem(glm::vec3(0.0f, 0.0f, 0.0f), explosion_particles_, &particle_shader_, tex_[9], game_objects_[j]);
                        particles->SetScale(0.2);
                        game_objects_.push_back(particles);

                        //deplete hp
                        PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(current_game_object);

                        //check if invincible
                        if (!player->IsInvincible()) {
                            player->SetPlayerHP(player->GetPlayerHP() - 1);
                        }

                        if (player->GetPlayerHP() <= 0) {
                            //KILL
                            player->Explode(tex_[4]);

                            //create explosion particle system here
                            GameObject* particles = new ParticleSystem(glm::vec3(0.0f, 0.0f, 0.0f), explosion_particles_, &particle_shader_, tex_[9], game_objects_[0]);
                            particles->SetScale(0.2);
                            game_objects_.push_back(particles);
                        }
                    }
                }
                //the smartest way i thought of checking for each enemy type
                else if (other_game_object->GetType() == GameObject::Enemy2 && other_game_object->GetState() == GameObject::Alive) {
                    //insert enemy object dynamic cast here
                    EnemyGameObject2* enemy = dynamic_cast<EnemyGameObject2*>(other_game_object);
                    //handle player collision (if its the player)
                    if (current_game_object->GetType() == GameObject::Player && enemy->GetState() == GameObject::Alive) {
                        //KILL ENEMY & increase score
                        enemy->Explode(tex_[4]);
                        score++;

                        //decrease hp
                        PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(current_game_object);
                        player->SetPlayerHP(player->GetPlayerHP() - 1);

                        //check if invincible
                        if (!player->IsInvincible()) {
                            player->SetPlayerHP(player->GetPlayerHP() - 1);
                        }

                        if (player->GetPlayerHP() <= 0) {
                            //KILL
                            player->Explode(tex_[4]);

                            //create explosion particle system here
                            GameObject* particles = new ParticleSystem(glm::vec3(0.0f, 0.0f, 0.0f), explosion_particles_, &particle_shader_, tex_[9], game_objects_[0]);
                            particles->SetScale(0.2);
                            game_objects_.push_back(particles);
                        }
                    }
                } 
                //the smartest way i thought of checking for each enemy type part 2
                else if (other_game_object->GetType() == GameObject::Enemy3 && other_game_object->GetState() == GameObject::Alive) {
                    //insert enemy object dynamic cast here
                    EnemyGameObject* enemy = dynamic_cast<EnemyGameObject*>(other_game_object);
                    //handle player collision (if its the player)
                    if (current_game_object->GetType() == GameObject::Player && enemy->GetState() == GameObject::Alive) {
                        //KILL ENEMY & increase score
                        enemy->Explode(tex_[4]);
                        score++;

                        //decrease hp
                        PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(current_game_object);
                        player->SetPlayerHP(player->GetPlayerHP() - 1);

                        //check if invincible
                        if (!player->IsInvincible()) {
                            player->SetPlayerHP(player->GetPlayerHP() - 1);
                        }

                        if (player->GetPlayerHP() <= 0) {
                            //KILL
                            player->Explode(tex_[4]);
                        }
                    }
                }
                //the smartest way i thought of checking for each enemy type part 3
                else if (other_game_object->GetType() == GameObject::Turret && other_game_object->GetState() == GameObject::Alive) {
                    //insert enemy object dynamic cast here
                    EnemyGameObject3* turret = dynamic_cast<EnemyGameObject3*>(other_game_object);
                    //handle player collision (if its the player)
                    if (current_game_object->GetType() == GameObject::Player && turret->GetState() == GameObject::Alive) {
                        //decrease hp
                        turret->SetTurretHP(turret->GetTurretHP() - 1);

                        //check if turret is dead
                        if (turret->GetTurretHP() <= 0) {
                            turret->Explode(tex_[4]);
                            score++;

                            //create explosion particle system here
                            GameObject* particles = new ParticleSystem(glm::vec3(0.0f, 0.0f, 0.0f), explosion_particles_, &particle_shader_, tex_[9], game_objects_[j]);
                            particles->SetScale(0.2);
                            game_objects_.push_back(particles);
                        }

                        //decrease hp
                        PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(current_game_object);
                        player->SetPlayerHP(player->GetPlayerHP() - 1);

                        //check if invincible
                        if (!player->IsInvincible()) {
                            player->SetPlayerHP(player->GetPlayerHP() - 1);
                        }

                        if (player->GetPlayerHP() <= 0) {
                            //KILL
                            player->Explode(tex_[4]);
                        }
                    }
                }
                //item check
                else if (other_game_object->GetType() == GameObject::Item) {
                    //create reference to player & item
                    ItemGameObject* item = dynamic_cast<ItemGameObject*>(other_game_object);
                    if (current_game_object->GetType() == GameObject::Player) {
                        PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(current_game_object);
                        item->Collected();
                        //hp power up
                        if (item->GetItemType().compare(item_1) == 0) {
                            player->SetPlayerHP(player->GetPlayerHP() + 1);
                            std::cout << "HP UP!" << std::endl;
                        }
                        //dmg up power up
                        else if (item->GetItemType().compare(item_2) == 0) {
                            player->SetPlayerDamage(player->GetPlayerDamage() + 1);
                            std::cout << "DMG UP!" << std::endl;
                        }
                        //invincible power up
                        else if (item->GetItemType().compare(item_3) == 0) {
                            //create a shield at the player's position
                            player->invincibility(tex_[11]);
                            std::cout << "INVINCIBLE!" << std::endl;
                        }
                    }
                }
            }
        }

        // Check for intersections betwen missiles and enemies
        if (current_game_object->GetType() == GameObject::Weapon) {
            // Get missile
            BulletGameObject* missile = dynamic_cast<BulletGameObject*>(current_game_object);
            // Check for collisions
            int index = missile->Collision(game_objects_);
            // If there is a collision, simply remove the game object
            if (index != -1) {
                missile->SetContact(true);
                if (game_objects_[index]->GetState() == GameObject::Alive) {
                    if (game_objects_[index]->GetType() != GameObject::Turret) {
                        game_objects_[index]->Explode(tex_[4]);
                        score++;

                        //create explosion particle system here
                        GameObject* particles = new ParticleSystem(glm::vec3(0.0f, -0.5f, 0.0f), explosion_particles_, &particle_shader_, tex_[9], game_objects_[index]);
                        particles->SetScale(0.2);
                        game_objects_.push_back(particles);
                    }
                    else {
                        EnemyGameObject3* turret = dynamic_cast<EnemyGameObject3*>(game_objects_[index]);

                        //decrease turret hp
                        turret->SetTurretHP(turret->GetTurretHP() - 1);

                        //check if turret is dead
                        if (turret->GetTurretHP() <= 0) {
                            turret->Explode(tex_[4]);
                            score++;

                            //create explosion particle system here
                            GameObject* particles = new ParticleSystem(glm::vec3(0.0f, -0.5f, 0.0f), explosion_particles_, &particle_shader_, tex_[9], game_objects_[index]);
                            particles->SetScale(0.2);
                            game_objects_.push_back(particles);
                        }
                    }
                }
            }
        }
        //check for intersections between enemy missile and player
        else if (current_game_object->GetType() == GameObject::EWeapon) {
            // Get missile
            EnemyBulletGameObject* missile = dynamic_cast<EnemyBulletGameObject*>(current_game_object);
            // Check for collisions
            int index = missile->Collision(game_objects_);
            // If there is a collision, simply remove the game object
            if (index != -1) {
                //delete missile so that it doesn't eat your health
                //also decrease hp
                missile->SetContact(true);
                PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(game_objects_[index]);

                //check if invincible
                if (!player->IsInvincible()) {
                    player->SetPlayerHP(player->GetPlayerHP() - 1);
                }

                if (player->GetPlayerHP() <= 0) {
                    //KILL
                    player->Explode(tex_[4]);
                }
            }
        }

        //update UI
        if (current_game_object->GetType() == GameObject::UI) {
            //get text & player
            TextGameObject* UI = dynamic_cast<TextGameObject*>(current_game_object);
            PlayerGameObject* player = dynamic_cast<PlayerGameObject*>(game_objects_[0]);
            if (UI->GetUI().compare("HP") == 0) {
                //update the text
                UI->SetText("Player HP: " + std::to_string(player->GetPlayerHP()));
            }
            else if (UI->GetUI().compare("Time") == 0) {
                //update the text
                UI->SetText("Time: " + std::to_string(current_time_));
            }
            else if (UI->GetUI().compare("Weapon") == 0) {
                UI->SetText("Weapon: " + player->GetWeaponType());
            }
            else {
                UI->SetText("Score: " + std::to_string(score));
            }
        }

        // Render game object
        current_game_object->Render(view_matrix, current_time_);
    }
}


void Game::Controls(double delta_time)
{
    // Get player game object
    GameObject* player = game_objects_[0];
    // Get current position & angle
    glm::vec3 curpos = player->GetPosition();
    float angle = player->GetAngle();
    // Compute current bearing direction
    glm::vec3 dir = player->GetBearing();
    // Adjust motion increment and angle increment 
    // if translation or rotation is too slow
    float speed = delta_time * 900.0;
    float motion_increment = 0.001 * speed;
    float angle_increment = (glm::pi<float>() / 1800.0f) * speed;

    // Check for player input and make changes accordingly
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        player->SetPosition(curpos + motion_increment * dir);
    }
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        player->SetPosition(curpos - motion_increment * dir);
    }
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        player->SetAngle(angle - angle_increment);
    }
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        player->SetAngle(angle + angle_increment);
    }
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        //indicate switch
        PlayerGameObject* p = dynamic_cast<PlayerGameObject*>(player);
        p->SetWeaponType("gun");
        // Check if we are allowed to fire now given the cool down time
        if (current_time_ > bullet_cooldown) {
            // Create and initialize the missile
            BulletGameObject* missile = new BulletGameObject(player->GetPosition(), sprite_, &sprite_shader_, tex_[8]);
            missile->SetVelocity(3.0f * player->GetBearing());
            missile->SetScale(0.3f);

            // Set the orientation of the bullet with the player's orientation
            missile->SetAngle(player->GetAngle());

            // Add missile to list of game objects
            // We should add it before the other unrelated game objects
            game_objects_.insert(game_objects_.begin() + 3, missile);

            // Set next allowed firing time
            bullet_cooldown = current_time_ + 0.5f;
        }
    }

    if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
        //indicate switch
        PlayerGameObject* p = dynamic_cast<PlayerGameObject*>(player);
        p->SetWeaponType("shotgun");
        // Check if we are allowed to fire now given the cool down time
        if (current_time_ > shotgun_cooldown) {
            // Create and initialize the missile
            BulletGameObject* missile = new BulletGameObject(player->GetPosition(), sprite_, &sprite_shader_, tex_[8]);
            BulletGameObject* missile2 = new BulletGameObject(player->GetPosition(), sprite_, &sprite_shader_, tex_[8]);
            BulletGameObject* missile3 = new BulletGameObject(player->GetPosition(), sprite_, &sprite_shader_, tex_[8]);
            BulletGameObject* missile4 = new BulletGameObject(player->GetPosition(), sprite_, &sprite_shader_, tex_[8]);
            BulletGameObject* missile5 = new BulletGameObject(player->GetPosition(), sprite_, &sprite_shader_, tex_[8]);

            // remaking the getBearing function here to get the actual velocity the bullets should travel at
            // Assumes sprite is initially rotated by 90 degrees
            float pi_over_two = glm::pi<float>() / 2.0f;


            //glm::vec3 dir(cos(angle_ + pi_over_two), sin(angle_ + pi_over_two), 0.0);
            float num = 0.785;
            glm::vec3 right(cos(player->GetAngle() + num + pi_over_two), sin(player->GetAngle() + num + pi_over_two), 0.0);
            glm::vec3 left(cos(player->GetAngle() - num + pi_over_two), sin(player->GetAngle() - num + pi_over_two), 0.0);
            glm::vec3 ne(cos(player->GetAngle() + num / 2 + pi_over_two), sin(player->GetAngle() + num / 2 + pi_over_two), 0.0);
            glm::vec3 nw(cos(player->GetAngle() - num / 2 + pi_over_two), sin(player->GetAngle() - num / 2 + pi_over_two), 0.0);

            missile->SetVelocity(3.0f * player->GetBearing());
            missile2->SetVelocity(3.0f * right);
            missile3->SetVelocity(3.0f * left);
            missile4->SetVelocity(3.0f * ne);
            missile5->SetVelocity(3.0f * nw);

            //missile2->SetVelocity(3.0f * (player->GetBearing()[0] + (float)1.57);
            //missile3->SetVelocity(3.0f * (player->GetBearing()[0] - (float)1.57));

            missile->SetScale(0.3f);
            missile2->SetScale(0.3f);
            missile3->SetScale(0.3f);
            missile4->SetScale(0.3f);
            missile5->SetScale(0.3f);

            // Set the orientation of the bullet with the player's orientation
            missile->SetAngle(player->GetAngle());
            missile2->SetAngle(player->GetAngle());
            missile3->SetAngle(player->GetAngle());
            missile4->SetAngle(player->GetAngle());
            missile5->SetAngle(player->GetAngle());

            // Add missile to list of game objects
            // We should add it before the unrelated game objects
            game_objects_.insert(game_objects_.begin() + 3, missile);
            game_objects_.insert(game_objects_.begin() + 3, missile2);
            game_objects_.insert(game_objects_.begin() + 3, missile3);
            game_objects_.insert(game_objects_.begin() + 3, missile4);
            game_objects_.insert(game_objects_.begin() + 3, missile5);

            // Set next allowed firing time
            shotgun_cooldown = current_time_ + 2.0f;

        }
    }

    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
    //test thing
    if (glfwGetKey(window_, GLFW_KEY_P) == GLFW_PRESS) {
        std::cout << player->GetPosition().x << " " << player->GetPosition().y << std::endl;
    }
}
       
} // namespace game
