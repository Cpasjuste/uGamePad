//
// Created by cpasjuste on 30/03/23.
//

#include <ctime>
#include "main.h"
#include "starfield.h"

using namespace uGamePad;

// Screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Starfield parameters
#define NUM_STARS 80
#define NUM_SHOOTING_STARS 2
#define MAX_STAR_SPEED 2.0f
#define MIN_STAR_SPEED 0.2f

static Adafruit_GFX *s_gfx;

typedef struct {
    float x;
    float y;
    float z; // z is for depth/speed
    float brightness;
} Star;

typedef struct {
    float x;
    float y;
    float length;
    float angle;
    float speed;
    int life;
    int max_life;
    bool active;
} ShootingStar;

Star stars[NUM_STARS];
ShootingStar shooting_stars[NUM_SHOOTING_STARS];

// Initialize random stars
void init_stars() {
    srand(time(NULL));

    for (auto &star: stars) {
        star.x = rand() % SCREEN_WIDTH;
        star.y = rand() % SCREEN_HEIGHT;
        star.z = (float) rand() / RAND_MAX * 0.8f + 0.2f; // Random depth between 0.2 and 1.0
        star.brightness = (float) rand() / RAND_MAX * 0.5f + 0.5f; // Random brightness
    }

    for (auto &shooting_star: shooting_stars) {
        shooting_star.active = false;
    }
}

// Update star positions
void update_stars() {
    for (auto &star: stars) {
        // Move stars based on depth (z value)
        star.x -= star.z * 0.5f;

        // Wrap around screen if needed
        if (star.x < 0) {
            star.x = SCREEN_WIDTH;
            star.y = rand() % SCREEN_HEIGHT;
            star.z = (float) rand() / RAND_MAX * 0.8f + 0.2f;
        }
    }

    // Update shooting stars
    for (auto &shooting_star: shooting_stars) {
        if (shooting_star.active) {
            // Update position based on angle and speed
            shooting_star.x += cos(shooting_star.angle) * shooting_star.speed;
            shooting_star.y += sin(shooting_star.angle) * shooting_star.speed;

            // Update life
            shooting_star.life--;
            if (shooting_star.life <= 0 ||
                shooting_star.x < 0 || shooting_star.x >= SCREEN_WIDTH ||
                shooting_star.y < 0 || shooting_star.y >= SCREEN_HEIGHT) {
                shooting_star.active = false;
            }
        } else {
            // Random chance to spawn a new shooting star
            if (rand() % 100 < 2) {
                // 2% chance per frame
                shooting_star.active = true;
                shooting_star.x = rand() % SCREEN_WIDTH;
                shooting_star.y = rand() % (SCREEN_HEIGHT / 2); // Start in top half
                shooting_star.angle = M_PI / 4 + ((float) rand() / RAND_MAX * M_PI / 4);
                // Angle between 45-90 degrees
                shooting_star.speed = (float) rand() / RAND_MAX * 2.0f + 1.0f;
                shooting_star.length = (float) rand() / RAND_MAX * 8.0f + 3.0f;
                shooting_star.max_life = rand() % 20 + 10;
                shooting_star.life = shooting_star.max_life;
            }
        }
    }
}

// Draw a line for shooting stars
void draw_shooting_star_line(int x0, int y0, int x1, int y1, float brightness) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        if (x0 >= 0 && x0 < SCREEN_WIDTH && y0 >= 0 && y0 < SCREEN_HEIGHT) {
            // Set pixel with probability based on brightness
            if ((float) rand() / RAND_MAX < brightness) {
                s_gfx->drawPixel(x0, y0, 1);
            }
        }

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
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

// Render the starfield
void render_starfield() {
    // Draw stars
    for (auto &star: stars) {
        int x = (int) star.x;
        int y = (int) star.y;

        if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
            // Brighter stars have a chance to be drawn larger
            s_gfx->drawPixel(x, y, 1);

            // For brighter stars, draw them larger based on brightness and z
            if (star.brightness > 0.8f && star.z > 0.7f) {
                // Draw a plus pattern for bright stars
                if (x + 1 < SCREEN_WIDTH) s_gfx->drawPixel(x + 1, y, 1);
                if (x - 1 >= 0) s_gfx->drawPixel(x - 1, y, 1);
                if (y + 1 < SCREEN_HEIGHT) s_gfx->drawPixel(x, y + 1, 1);
                if (y - 1 >= 0) s_gfx->drawPixel(x, y - 1, 1);
            }
        }
    }

    // Draw shooting stars
    for (auto &shooting_star: shooting_stars) {
        if (shooting_star.active) {
            float brightness = (float) shooting_star.life / shooting_star.max_life;
            int tail_x = shooting_star.x - cos(shooting_star.angle) * shooting_star.length;
            int tail_y = shooting_star.y - sin(shooting_star.angle) * shooting_star.length;

            draw_shooting_star_line((int) shooting_star.x, (int) shooting_star.y,
                                    tail_x, tail_y, brightness);
        }
    }
}

StarFieldBg::StarFieldBg(int16_t x, int16_t y, int16_t w, int16_t h, const Utility::Color &color) : Widget() {
    Widget::setPosition(x, y);
    Widget::setSize(w, h);
    m_color = color;

    s_gfx = StarFieldBg::getGfx();
    init_stars();
}

void StarFieldBg::loop(const Utility::Vec2i &pos) {
    update_stars();
    render_starfield();

    Widget::loop(pos);
}
