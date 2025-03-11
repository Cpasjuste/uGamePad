//
// Created by cpasjuste on 30/03/23.
//

#include <ctime>
#include "main.h"
#include "particles.h"

using namespace uGamePad;

#define NUM_PARTICLES 20
#define MIN_SPEED 0.2
#define MAX_SPEED 0.8
#define CONNECTION_DISTANCE 20

typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    int life;
    int max_life;
} Particle;

Particle particles[NUM_PARTICLES];

// draw line between two points using Bresenham's algorithm
static void DrawLine(Adafruit_GFX *gfx, int x0, int y0, const int x1, const int y1) {
    const int dx = abs(x1 - x0);
    const int sx = x0 < x1 ? 1 : -1;
    const int dy = -abs(y1 - y0);
    const int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        if (x0 >= 0 && x0 < gfx->width() && y0 >= 0 && y0 < gfx->height()) {
            gfx->drawPixel(x0, y0, 1);
        }

        if (x0 == x1 && y0 == y1) break;

        const int e2 = 2 * err;
        if (e2 >= dy) {
            if (x0 == x1) break;
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            if (y0 == y1) break;
            err += dx;
            y0 += sy;
        }
    }
}

ParticleBg::ParticleBg(int16_t x, int16_t y, int16_t w, int16_t h, const Utility::Color &color) : Widget() {
    Widget::setPosition(x, y);
    Widget::setSize(w, h);
    m_color = color;

    srand(time(nullptr));
    for (auto &particle: particles) {
        particle.x = rand() % getSize().x;
        particle.y = rand() % getSize().y;

        // random velocity
        particle.vx = ((float) rand() / RAND_MAX * (MAX_SPEED - MIN_SPEED) + MIN_SPEED) *
                      (rand() % 2 ? 1 : -1);
        particle.vy = ((float) rand() / RAND_MAX * (MAX_SPEED - MIN_SPEED) + MIN_SPEED) *
                      (rand() % 2 ? 1 : -1);

        // random lifetime (some particles live longer than others)
        particle.max_life = rand() % 200 + 100;
        particle.life = rand() % particle.max_life;
    }
}

void ParticleBg::loop(const Utility::Vec2i &pos) {
    // update particle positions and handle boundaries
    for (auto &particle: particles) {
        // Update position
        particle.x += particle.vx;
        particle.y += particle.vy;

        // increment life and reset if needed
        particle.life++;
        if (particle.life >= particle.max_life) {
            // reset particle with new parameters
            particle.x = rand() % getSize().x;
            particle.y = rand() % getSize().y;
            particle.vx = ((float) rand() / RAND_MAX * (MAX_SPEED - MIN_SPEED) + MIN_SPEED) *
                          (rand() % 2 ? 1 : -1);
            particle.vy = ((float) rand() / RAND_MAX * (MAX_SPEED - MIN_SPEED) + MIN_SPEED) *
                          (rand() % 2 ? 1 : -1);
            particle.life = 0;
        }

        // bounce off edges
        if (particle.x < 0 || particle.x >= getSize().x) {
            particle.vx *= -1;
            particle.x = fmax(0, fmin(particle.x, getSize().x - 1));
        }

        if (particle.y < 0 || particle.y >= getSize().y) {
            particle.vy *= -1;
            particle.y = fmax(0, fmin(particle.y, getSize().y - 1));
        }
    }

    // render particles and connections

    // draw connection lines between nearby particles
    for (int i = 0; i < NUM_PARTICLES; i++) {
        for (int j = i + 1; j < NUM_PARTICLES; j++) {
            float dx = particles[i].x - particles[j].x;
            float dy = particles[i].y - particles[j].y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance < CONNECTION_DISTANCE) {
                // Draw connection line with intensity based on distance
                float intensity = 1.0f - (distance / CONNECTION_DISTANCE);
                if (rand() % 100 < intensity * 100) {
                    DrawLine(getGfx(), (int) particles[i].x, (int) particles[i].y,
                             (int) particles[j].x, (int) particles[j].y);
                }
            }
        }
    }

    // draw particle dots
    for (auto &particle: particles) {
        int x = (int) particle.x;
        int y = (int) particle.y;

        if (x >= 0 && x < getSize().x && y >= 0 && y < getSize().y) {
            getGfx()->drawPixel(x, y, 1);
        }
    }

    Widget::loop(pos);
}
