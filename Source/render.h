#pragma once
#include <raylib-ext.hpp>
#include "components.h"

struct Viewport
{
    float left, right;
    float bottom, top;
};

Vector2
screen_to_local(Viewport& viewport, Vector2 screen_size, Vector2 point);

void
set_viewport(Viewport& viewport, Vector2 screen_size, Rectangle rect);

std::vector<Entity *>
create_draw_order(std::vector<Entity> &entities);
