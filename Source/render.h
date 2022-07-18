#include <raylib-ext.hpp>


struct Viewport
{
    float left, right;
    float bottom, top;
};

Vector2
screen_to_local(Viewport& viewport, Vector2 screen_size, Vector2 point);

void
set_viewport(Viewport& viewport, Vector2 screen_size, Rectangle rect);