#ifndef PARTICLES_H_
#define PARTICLES_H_

#include "geometry.h"

#define NUM_PARTICLES 4000
#define NUM_EXPLOSION_PARTICLES 400

namespace game {

    // A set of particles that can be rendered
    class Particles : public Geometry {

        public:
            Particles(void);

            // Create the geometry (called once)
            void CreateGeometry(void);

            // create an explosion type geometry
            void CreateExplosionGeometry(void);

            // Use the geometry
            void SetGeometry(GLuint shader_program);

    }; // class Particles
} // namespace game

#endif // PARTICLES_H_
