#include "simulation.h"
#include <SDL.h>
#include <cstdlib>  // For rand()
#include <cmath>    // For cos, sin, M_PI
#include <ctime>    // For time()

Simulation::Simulation() : running(false) {
    srand(time(nullptr));
}

void Simulation::start() {
    running = true;
}

void Simulation::stop() {
    running = false;
}

void Simulation::reset() {
    particles.clear();
    for (int i = 0; i < 100; ++i) {
        particles.push_back(createRandomParticle());
    }
}

void Simulation::update(double deltaTime) {
    if (!running) return;  // Do not update if simulation is not running

    std::vector<Vec2> forces = physics.computeForces(particles);
    for (size_t i = 0; i < particles.size(); i++) {
        particles[i].update(forces[i], deltaTime);
    }
}

void Simulation::render(SDL_Renderer* renderer) {
    for (size_t i = 0; i < particles.size(); i++) {
        particles[i].render(renderer);
    }
}

Particle Simulation::createRandomParticle() {
    const int windowWidth = 1200; // Window width remains the same
    const int windowHeight = 800; // Window height remains the same

    // Generate position without excluding any part for control width
    float x = float(rand() % windowWidth);
    float y = float(rand() % windowHeight);
    Vec2 pos(x, y);

    // Angle and speed calculations remain unchanged
    float angle = float(rand() % 360) * M_PI / 180.0f;
    float speed = float(rand() % 50) / 10.0f;
    Vec2 vel(cos(angle) * speed, sin(angle) * speed);

    // Color generation remains unchanged
    SDL_Color color = {Uint8(rand() % 256), Uint8(rand() % 256), Uint8(rand() % 256), 255};
    float radius = 5.0f;
    float mass = radius / 5.0f;

    return Particle(pos, vel, color, radius, mass, 0.0, 0.0, 0);
}

void Simulation::handleEvent(const SDL_Event& event) {
    // Example: Respond to keyboard events to toggle simulation parameters
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_g:
                // Toggle gravity effect
                physics.toggleGravity();
                break;
            case SDLK_p:
                // Add or remove particles
                if (particles.size() < 1000) {
                    particles.push_back(createRandomParticle());
                } else {
                    particles.pop_back();
                }
                break;
        }
    }
}

int Simulation::getParticleCount() const {
    return particles.size();
}

float Simulation::getFrameRate() const {
    // For now, return a fixed frame rate; replace with dynamic calculation if needed
    return 60.0f;  // Assuming 60 FPS for demonstration purposes
}
