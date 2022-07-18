#include "render.h"

Vector2
screen_to_local(Viewport& viewport, Vector2 screen_size, Vector2 point)
{
    Vector2 d = {
        viewport.right - viewport.left,
        viewport.top - viewport.bottom
    };
    return Vector2{
        (point.x / screen_size.x) * d.x + viewport.left,
        (point.y / screen_size.y) * d.y + viewport.bottom,
    };
}

void
set_viewport(Viewport& viewport, Vector2 screen_size, Rectangle rect)
{
    Vector2 p0 = screen_to_local(viewport, screen_size, Vector2{ rect.x, rect.y });
    Vector2 p1 = screen_to_local(viewport, screen_size, Vector2{ rect.x + rect.width, rect.y + rect.height });
    viewport = Viewport{
        p0.x,
        p1.x,
        p0.y,
        p1.y,
    };
}
