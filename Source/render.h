#pragma once
#include <raylib-ext.hpp>
#include "components.h"

namespace plat {

struct Viewport
{
    float left, right;
    float bottom, top;
};

Vector2
global_to_screen(Viewport& viewport, Vector2 screen_size, Vector2 point);

void
set_viewport(Viewport& viewport, Vector2 screen_size, Rectangle rect);

std::vector<Entity *>
create_draw_order(std::vector<Entity> &entities);

}
