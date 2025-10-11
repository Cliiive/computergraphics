#include "sdl2_renderer.h"
#include <span>
#include <utility>

void SDL2Renderer::renderSpaceship(Vector2df position, float angle) {
    static std::array<SDL_Point, 16> ship_points{
        SDL_Point{14, 0},     // Nose
        SDL_Point{8, -2},     // Front body right
        SDL_Point{2, -2},     // Mid body right
        SDL_Point{-2, -6},    // Rear wing upper right
        SDL_Point{-10, -12},  // Wing tip upper right
        SDL_Point{-12, -10},  // Engine upper right
        SDL_Point{-6, -6},    // Inner wing upper right
        SDL_Point{-6, -2},    // Rear body right
        SDL_Point{-10, 0},    // Engine rear
        SDL_Point{-6, 2},     // Rear body left
        SDL_Point{-6, 6},     // Inner wing lower right
        SDL_Point{-12, 10},   // Engine lower right
        SDL_Point{-10, 12},   // Wing tip lower right
        SDL_Point{-2, 6},     // Rear wing lower right
        SDL_Point{2, 2},      // Mid body left
        SDL_Point{8, 2}       // Front body left
    };

    std::array<SDL_Point, ship_points.size()> points;

    float cos_angle = std::cos(angle);
    float sin_angle = std::sin(angle);
    for (size_t i = 0; i < ship_points.size(); i++) {
        float x     = ship_points[i].x;
        float y     = ship_points[i].y;
        points[i].x = (cos_angle * x - sin_angle * y) + position[0];
        points[i].y = (sin_angle * x + cos_angle * y) + position[1];
    }
    SDL_SetRenderDrawColor(renderer, 80, 80, 255, 255);
    SDL_RenderDrawLines(renderer, points.data(), points.size());


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void SDL2Renderer::renderDeathStar(Vector2df position) {
    static SDL_Point death_star_points[] = {            // Main sphere outline - 24 points
                                            {0, -60},   // Top
                                            {15, -58},  // Outer circle points
                                            {30, -52},
                                            {42, -42},
                                            {52, -30},
                                            {58, -15},
                                            {60, 0},  // Right
                                            {58, 15},
                                            {52, 30},
                                            {42, 42},
                                            {30, 52},
                                            {15, 58},
                                            {0, 60},  // Bottom
                                            {-15, 58},
                                            {-30, 52},
                                            {-42, 42},
                                            {-52, 30},
                                            {-58, 15},
                                            {-60, 0},  // Left
                                            {-58, -15},
                                            {-52, -30},
                                            {-42, -42},
                                            {-30, -52},
                                            {-15, -58},
                                            {0, -60},  // Back to top

                                            // Superlaser dish (inner circle) - 16 points
                                            {-45, 0},  // Left edge of dish
                                            {-44, -9},
                                            {-40, -17},
                                            {-34, -24},
                                            {-24, -28},
                                            {-14, -30},
                                            {-4, -28},
                                            {4, -24},
                                            {10, -17},
                                            {12, -9},
                                            {12, 0},
                                            {10, 9},
                                            {4, 17},
                                            {-4, 24},
                                            {-14, 28},
                                            {-24, 28},
                                            {-34, 24},
                                            {-40, 17},
                                            {-44, 9},
                                            {-45, 0},  // Close inner circle

                                            // Stripes pointing to the center of the dish
                                            {-45, -10},  // Top stripe
                                            {12, -5},    // Point toward dish center
                                            {-45, 0},    // Middle stripe
                                            {12, 0},     // Points straight to center
                                            {-45, 10},   // Bottom stripe
                                            {12, 5}};    // Point toward dish center

    std::array<SDL_Point, std::span{death_star_points}.size()> points;

    for (size_t i = 0; i < points.size(); i++) {
        float x     = death_star_points[i].x;
        float y     = death_star_points[i].y;
        points[i].x = x + position[0];
        points[i].y = y + position[1];
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLines(renderer, points.data(), points.size());
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void SDL2Renderer::render(Spaceship* ship) {
  // Main thruster flame
  static SDL_Point flame_points[]{{-6, 3}, {-12, 0}, {-6, -3}};
  // Additional flame stripes for more dynamic effect
  static SDL_Point flame_stripes1[]{{-6, 2}, {-14, 0}, {-6, -2}};
  static SDL_Point flame_stripes2[]{{-6, 1}, {-10, 0}, {-6, -1}};
  
  std::array<SDL_Point, std::span{flame_points}.size()> points;
  std::array<SDL_Point, std::span{flame_stripes1}.size()> stripes1;
  std::array<SDL_Point, std::span{flame_stripes2}.size()> stripes2;

  if (!ship->is_in_hyperspace()) {
    if (ship->is_accelerating()) {
      float cos_angle = std::cos(ship->get_angle());
      float sin_angle = std::sin(ship->get_angle());
      
      // Transform main flame points
      for (size_t i = 0; i < points.size(); i++) {
        float x     = flame_points[i].x;
        float y     = flame_points[i].y;
        points[i].x = (cos_angle * x - sin_angle * y) + ship->get_position()[0];
        points[i].y = (sin_angle * x + cos_angle * y) + ship->get_position()[1];
      }
      
      // Transform additional flame stripes
      for (size_t i = 0; i < stripes1.size(); i++) {
        float x      = flame_stripes1[i].x;
        float y      = flame_stripes1[i].y;
        stripes1[i].x = (cos_angle * x - sin_angle * y) + ship->get_position()[0];
        stripes1[i].y = (sin_angle * x + cos_angle * y) + ship->get_position()[1];
        
        x = flame_stripes2[i].x;
        y = flame_stripes2[i].y;
        stripes2[i].x = (cos_angle * x - sin_angle * y) + ship->get_position()[0];
        stripes2[i].y = (sin_angle * x + cos_angle * y) + ship->get_position()[1];
      }
      
      // Draw the flames with varying colors
      SDL_SetRenderDrawColor(renderer, 255, 60, 0, 255); // Bright orange
      SDL_RenderDrawLines(renderer, points.data(), points.size());
      
      SDL_SetRenderDrawColor(renderer, 255, 120, 0, 200); // Medium orange
      SDL_RenderDrawLines(renderer, stripes1.data(), stripes1.size());
      
      SDL_SetRenderDrawColor(renderer, 255, 180, 0, 150); // Light orange/yellow
      SDL_RenderDrawLines(renderer, stripes2.data(), stripes2.size());
      
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
    renderSpaceship(ship->get_position(), ship->get_angle());
  }
}

void SDL2Renderer::render(Saucer* saucer) {
    static SDL_Point saucer_points[] = {
        {-32, -24},  // Left wing top corner
        {-32, 24},   // Left wing bottom corner
        {-16, 12},   // Left wing inner bottom
        {-8, 8},     // Cockpit bottom left
        {0, 12},     // Cockpit bottom center
        {8, 8},      // Cockpit bottom right
        {16, 12},    // Right wing inner bottom
        {32, 24},    // Right wing bottom corner
        {32, -24},   // Right wing top corner
        {16, -12},   // Right wing inner top
        {8, -8},     // Cockpit top right
        {0, -12},    // Cockpit top center
        {-8, -8},    // Cockpit top left
        {-16, -12},  // Left wing inner top
        {-32, -24}   // Back to left wing top corner
    };

    std::array<SDL_Point, std::span{saucer_points}.size()> points;

    Vector2df position = saucer->get_position();
    float     scale    = 0.5;
    if (saucer->get_size() == 0) {
        scale = 0.25;
    }
    for (size_t i = 0; i < points.size(); i++) {
        float x     = saucer_points[i].x;
        float y     = saucer_points[i].y;
        points[i].x = scale * x + position[0];
        points[i].y = scale * y + position[1];
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLines(renderer, points.data(), points.size());
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void SDL2Renderer::render(Torpedo* torpedo) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawPoint(renderer, torpedo->get_position()[0], torpedo->get_position()[1]);
    SDL_RenderDrawPoint(renderer, torpedo->get_position()[0] + 1, torpedo->get_position()[1]);
    SDL_RenderDrawPoint(renderer, torpedo->get_position()[0], torpedo->get_position()[1] - 1);
    SDL_RenderDrawPoint(renderer, torpedo->get_position()[0], torpedo->get_position()[1] + 1);
    SDL_RenderDrawPoint(renderer, torpedo->get_position()[0] - 1, torpedo->get_position()[1]);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void SDL2Renderer::render(Asteroid* asteroid) {
    // Type 1: Large asteroid with cratered surface and jagged edges
    static SDL_Point asteroids_points1[] = {
        {0, -12},   {8, -18},   {16, -24}, {24, -20}, {32, -12}, {28, -6},  {32, 0},
        {28, 6},    {32, 12},   {24, 18},  {16, 20},  {8, 24},   {0, 22},   {-8, 24},
        {-16, 24},  {-24, 20},  {-32, 12}, {-30, 4},  {-36, 0},  {-30, -4}, {-32, -12},
        {-24, -18}, {-16, -24}, {-8, -20}, {0, -12}};

    // Type 2: Elongated asteroid with impact craters and protrusions
    static SDL_Point asteroids_points2[] = {
        {16, -6},  {24, -10},  {32, -12},  {28, -18},  {16, -24}, {8, -20},  {0, -16},
        {-8, -20}, {-16, -24}, {-24, -18}, {-28, -12}, {-24, -6}, {-16, -0}, {-24, 6},
        {-32, 12}, {-24, 18},  {-16, 24},  {-8, 20},   {0, 18},   {8, 22},   {16, 24},
        {24, 18},  {32, 6},    {28, 0},    {16, -6}};

    // Type 3: Diamond-shaped asteroid with irregular surface features
    static SDL_Point asteroids_points3[] = {
        {-16, 0}, {-24, 4}, {-32, 6},  {-28, 12},  {-20, 18},  {-16, 24}, {-8, 20},  {0, 24},
        {8, 20},  {16, 24}, {24, 18},  {32, 6},    {28, 0},    {32, -6},  {24, -12}, {16, -18},
        {8, -20}, {0, -24}, {-8, -20}, {-16, -16}, {-24, -12}, {-32, -6}, {-24, -2}, {-16, 0}};

    // Type 4: Irregular boulder with multiple facets and cavities
    static SDL_Point asteroids_points4[] = {
        {8, 0},   {16, -4}, {32, -6},  {28, -10},  {32, -12},  {24, -18},  {16, -22},
        {8, -24}, {0, -22}, {-8, -24}, {-16, -24}, {-24, -18}, {-32, -12}, {-28, -6},
        {-32, 0}, {-28, 6}, {-32, 12}, {-24, 18},  {-16, 24},  {-8, 20},   {0, 22},
        {8, 16},  {16, 24}, {24, 18},  {32, 12},   {28, 6},    {24, 0},    {8, 0}};
    static size_t sizes[] = {
        std::span{asteroids_points1}.size(), std::span{asteroids_points2}.size(),
        std::span{asteroids_points3}.size(), std::span{asteroids_points4}.size()};
    size_t     size             = sizes[asteroid->get_rock_type()];
    SDL_Point* asteroids_points = asteroids_points1;
    if (asteroid->get_rock_type() == 1)
        asteroids_points = asteroids_points2;
    if (asteroid->get_rock_type() == 2)
        asteroids_points = asteroids_points3;
    if (asteroid->get_rock_type() == 3)
        asteroids_points = asteroids_points4;

    SDL_Point points[std::span{asteroids_points4}.size()];

    float     scale = (asteroid->get_size() == 3 ? 1.0 : (asteroid->get_size() == 2 ? 0.5 : 0.25));
    Vector2df position = asteroid->get_position();
    for (size_t i = 0; i < size; i++) {
        points[i].x = scale * asteroids_points[i].x + position[0];
        points[i].y = scale * asteroids_points[i].y + position[1];
    }
    SDL_RenderDrawLines(renderer, points, size);
}

void SDL2Renderer::render(SpaceshipDebris* debris) {
    // Get explosion progress (1.0 = new explosion, 0.0 = about to disappear)
    float     progress = debris->get_time_to_delete() / SpaceshipDebris::TIME_TO_DELETE;
    Vector2df position = debris->get_position();

    // Explosion shockwave ring
    const int   segments   = 20;
    const float max_radius = 40.0f;
    float       radius     = max_radius * (1.0f - progress);

    // Draw shockwave ring with fading intensity
    int alpha = static_cast<int>(255 * progress);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);

    for (int i = 0; i < segments; i++) {
        float angle1 = (i / static_cast<float>(segments)) * 2 * M_PI;
        float angle2 = ((i + 1) / static_cast<float>(segments)) * 2 * M_PI;

        int x1 = position[0] + radius * std::cos(angle1);
        int y1 = position[1] + radius * std::sin(angle1);
        int x2 = position[0] + radius * std::cos(angle2);
        int y2 = position[1] + radius * std::sin(angle2);

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    // Particle effects - flying debris
    const int num_particles = 12;
    for (int i = 0; i < num_particles; i++) {
        float angle        = (i / static_cast<float>(num_particles)) * 2 * M_PI;
        float speed_factor = 0.5f + static_cast<float>(i % 3) / 2.0f;  // Varied speeds

        float particle_dist = (1.0f - progress) * 60.0f * speed_factor;

        int x1 = position[0] + (particle_dist * 0.5f) * std::cos(angle);
        int y1 = position[1] + (particle_dist * 0.5f) * std::sin(angle);
        int x2 = position[0] + particle_dist * std::cos(angle);
        int y2 = position[1] + particle_dist * std::sin(angle);

        // Set color based on particle and explosion age (yellow->orange->red)
        if (progress > 0.7f) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, alpha);  // Bright yellow
        } else if (progress > 0.4f) {
            SDL_SetRenderDrawColor(renderer, 255, 128, 0, alpha);  // Orange
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, alpha);  // Red
        }

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    // Reset color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void SDL2Renderer::render(Debris* debris) {
    static SDL_Point debris_points[] = {{-32, 32}, {-32, -16}, {-16, 0},  {-16, -32}, {-8, 24},
                                        {8, -24},  {24, 32},   {24, -24}, {24, -32},  {32, -8}};

    static SDL_Point point;
    Vector2df        position = debris->get_position();
    for (size_t i = 0; i < std::span{debris_points}.size(); i++) {
        point.x = (Debris::TIME_TO_DELETE - debris->get_time_to_delete()) * debris_points[i].x +
                  position[0];
        point.y = (Debris::TIME_TO_DELETE - debris->get_time_to_delete()) * debris_points[i].y +
                  position[1];
        SDL_RenderDrawPoint(renderer, point.x, point.y);
    }
}

void SDL2Renderer::renderFreeShips() {
    constexpr float FREE_SHIP_X = 128;
    constexpr float FREE_SHIP_Y = 64;
    Vector2df       position    = {FREE_SHIP_X, FREE_SHIP_Y};

    for (int i = 0; i < game.get_no_of_ships(); i++) {
        renderSpaceship(position, -PI / 2.0);
        position[0] += 20.0;
    }
}

void SDL2Renderer::renderScore() {
    constexpr float SCORE_X = 128 - 48;
    constexpr float SCORE_Y = 48 - 4;

    static SDL_Point digit_0[] = {{0, -8}, {4, -8}, {4, 0}, {0, 0}, {0, -8}};
    static SDL_Point digit_1[] = {{4, 0}, {4, -8}};
    static SDL_Point digit_2[] = {{0, -8}, {4, -8}, {4, -4}, {0, -4}, {0, 0}, {4, 0}};
    static SDL_Point digit_3[] = {{0, 0}, {4, 0}, {4, -4}, {0, -4}, {4, -4}, {4, -8}, {0, -8}};
    static SDL_Point digit_4[] = {{4, 0}, {4, -8}, {4, -4}, {0, -4}, {0, -8}};
    static SDL_Point digit_5[] = {{0, 0}, {4, 0}, {4, -4}, {0, -4}, {0, -8}, {4, -8}};
    static SDL_Point digit_6[] = {{0, -8}, {0, 0}, {4, 0}, {4, -4}, {0, -4}};
    static SDL_Point digit_7[] = {{0, -8}, {4, -8}, {4, 0}};
    static SDL_Point digit_8[] = {{0, -8}, {4, -8}, {4, 0}, {0, 0}, {0, -8}, {0, -4}, {4, -4}};
    static SDL_Point digit_9[] = {{4, 0}, {4, -8}, {0, -8}, {0, -4}, {4, -4}};

    static size_t     sizes[]  = {std::span{digit_0}.size(), std::span{digit_1}.size(),
                                  std::span{digit_2}.size(), std::span{digit_3}.size(),
                                  std::span{digit_4}.size(), std::span{digit_5}.size(),
                                  std::span{digit_6}.size(), std::span{digit_7}.size(),
                                  std::span{digit_8}.size(), std::span{digit_9}.size()};
    static SDL_Point* digits[] = {digit_0, digit_1, digit_2, digit_3, digit_4,
                                  digit_5, digit_6, digit_7, digit_8, digit_9};

    std::array<SDL_Point, 7> points;
    long long                score        = game.get_score();
    int                      no_of_digits = 0;
    if (score > 0) {
        no_of_digits = std::trunc(std::log10(score)) + 1;
    }
    size_t x = SCORE_X + 20 * no_of_digits;
    size_t y = SCORE_Y;

    do {
        int d = score % 10;
        score /= 10;
        size_t size = sizes[d];
        for (size_t i = 0; i < size; i++) {
            points[i].x = x + 4 * (digits[d] + i)->x;
            points[i].y = y + 4 * (digits[d] + i)->y;
        }
        x -= 20;
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
        SDL_RenderDrawLines(renderer, points.data(), size);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        no_of_digits--;
    } while (no_of_digits > 0);
}

bool SDL2Renderer::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    } else {
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  window_width, window_height, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        } else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == nullptr) {
                std::cout << "No renderer from window: " << SDL_GetError() << std::endl;
            }
            return true;
        }
    }
    return false;
}

void SDL2Renderer::render() {

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    renderDeathStar(Vector2df{700.0f, 100.0f});  // Top right corner

    for (Body2df* body : game.get_physics().get_bodies()) {
        TypedBody* typed_body = static_cast<TypedBody*>(body);
        auto       type       = typed_body->get_type();
        if (type == BodyType::spaceship) {
            render(static_cast<Spaceship*>(typed_body));
        } else if (type == BodyType::torpedo) {
            render(static_cast<Torpedo*>(typed_body));
        } else if (type == BodyType::asteroid) {
            render(static_cast<Asteroid*>(typed_body));
        } else if (type == BodyType::spaceship_debris) {
            render(static_cast<SpaceshipDebris*>(typed_body));
        } else if (type == BodyType::debris) {
            render(static_cast<Debris*>(typed_body));
        } else if (type == BodyType::saucer) {
            render(static_cast<Saucer*>(typed_body));
        }
    }

    renderFreeShips();
    renderScore();
    SDL_RenderPresent(renderer);
}

void SDL2Renderer::exit() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}
