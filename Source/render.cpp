#include "render.h"
#include "components.h"

Vector2
global_to_screen(Viewport& viewport, Vector2 screen_size, Vector2 point)
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
    Vector2 p0 = global_to_screen(viewport, screen_size, Vector2{ rect.x, rect.y });
    Vector2 p1 = global_to_screen(viewport, screen_size, Vector2{ rect.x + rect.width, rect.y + rect.height });
    viewport = Viewport{
        p0.x,
        p1.x,
        p0.y,
        p1.y,
    };
}

std::vector<Entity *>
create_draw_order(std::vector<Entity> &entities)
{
    std::vector<Entity *> draw_queue;
    for (int i = 0; i < entities.size(); ++i)
    {
        draw_queue.push_back(&(entities[i]));
    }

    std::sort(draw_queue.begin(), draw_queue.end(),
        [](Entity *a, Entity *b) {
            float az = a->getComponent<_Transform>()->pos.z;
            float bz = b->getComponent<_Transform>()->pos.z;
            return az < bz;
    });

    return draw_queue;
}
