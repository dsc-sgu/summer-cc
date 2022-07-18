#define RAYEXT_IMPLEMENTATION
#include <raylib-ext.hpp>
#include "components.h"
#include <iostream>
#include<algorithm>
Storage storage;

struct Viewport
{
    float left, right;
    float bottom, top;
};

Vector2
screen_to_local(Viewport &viewport, Vector2 screen_size, Vector2 point)
{
    Vector2 d = {
        viewport.right - viewport.left,
        viewport.top - viewport.bottom
    };
    return Vector2 {
        (point.x / screen_size.x) * d.x + viewport.left,
        (point.y / screen_size.y) * d.y + viewport.bottom,
    };
}

void
set_viewport(Viewport &viewport, Vector2 screen_size, Rectangle rect)
{
    Vector2 p0 = screen_to_local(viewport, screen_size, Vector2 { rect.x, rect.y });
    Vector2 p1 = screen_to_local(viewport, screen_size, Vector2 { rect.x + rect.width, rect.y + rect.height });
    viewport = Viewport {
        p0.x,
        p1.x,
        p0.y,
        p1.y,
    };
}
struct {
    bool operator()(Entity * a, Entity * b) const { 
        int x = a->getComponent<_Transform>()->pos.z;
        int y = b->getComponent<_Transform>()->pos.z;
        return x < y; 
    }
} customLess;


void z_order(Storage strg, bool ord = true)
{
        
    if (ord){
        std::sort(strg.entities.begin(), strg.entities.end(), customLess);
    } else {
         std::sort(strg.entities.begin(), strg.entities.end(), customLess);
    }
}

int
main()
{
    const Vector2 screen_size { 640, 480 };
    InitWindow(screen_size.x, screen_size.y, "Creative Coding: Platformer");
    SetTargetFPS(60);

    Viewport viewport = {0, screen_size.x, screen_size.y, 0};
    set_viewport(viewport, screen_size, { 0, 0, screen_size.x, screen_size.y });


storage.entities.push_back(Entity());
    _Player_control *player_control = new _Player_control();
    _Transform *trnsf = new _Transform();
    _Sprite *spr = new _Sprite("Asssets/Textures/King_stay.png");
    player_control->speed = 100;
    storage.entities[0].components.push_back(player_control);
    storage.entities[0].components.push_back(trnsf);
    storage.entities[0].components.push_back(spr);





    storage.entities.push_back(Entity());
    _Transform *trnsf1 = new _Transform();
    _Sprite *spr1 = new _Sprite("Asssets/Textures/bg.png");
    storage.entities[1].components.push_back(trnsf1);
    storage.entities[1].components.push_back(spr1);

    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(BLACK);
                z_order(storage);
            for (int i = 0; i < storage.entities.size(); i++)
            {
                for (int j = 0; j < storage.entities[i].components.size(); j++)
                {
                    storage.entities[i].components[j]->Update(GetFrameTime(),storage.entities[i].id,storage);
                    auto t = storage.entities[i].getComponent<_Transform>();
                    auto new_pos = screen_to_local(viewport, screen_size, { t->pos.x, t->pos.y });
                    auto s = storage.entities[i].getComponent<_Sprite>();
                    DrawTexture(s->texture, new_pos.x, new_pos.y - s->texture.height, WHITE);
                }
            }

            if (IsKeyDown(KEY_LEFT))
            {
                viewport.left += 1;
                viewport.right += 1;
            }
            if (IsKeyDown(KEY_RIGHT))
            {
                viewport.left -= 1;
                viewport.right -= 1;
            }
            if (IsKeyDown(KEY_UP))
            {
                viewport.top += 1;
                viewport.bottom += 1;
            }
            if (IsKeyDown(KEY_DOWN))
            {
                viewport.top -= 1;
                viewport.bottom -= 1;
            }

            DrawFPS(10, 10);
        }

        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}

