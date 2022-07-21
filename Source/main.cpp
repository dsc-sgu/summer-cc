#define RAYEXT_IMPLEMENTATION
#include <raylib-ext.hpp>
#include <iostream>
#include <algorithm>
#include "json_loader.h"
#include "render.h"

int
main()
{
    const Vector2 screen_size { 720, 480 };
    InitWindow(screen_size.x, screen_size.y, "Creative Coding: Platformer");
    SetTargetFPS(60);

    plat::Storage storage = load_lvl("Assets/Scenes/default.json");
    std::vector<plat::Entity *> draw_queue = create_draw_order(storage.entities);

    while (!WindowShouldClose())
    {
        for (auto &entity : storage.entities)
        {
            for (auto &component : entity.components)
            {
                component->update(GetFrameTime(), entity.id, storage);
            }
        }

        plat::Camera *cam = storage.entities[storage.cur_camera].getComponent<plat::Camera>();
        plat::Transform *cam_t = storage.entities[storage.cur_camera].getComponent<plat::Transform>();

        BeginDrawing();
        ClearBackground(BLACK);
        for (int i = 0; i < draw_queue.size(); ++i)
        {
            for (int j = 0; j < draw_queue[i]->components.size(); ++j)
            {
                plat::Sprite *spr = draw_queue[i]->getComponent<plat::Sprite>();
                if (spr)
                {
                    plat::Transform *t = draw_queue[i]->getComponent<plat::Transform>();

                    Vector2 screen_pos = {
                        (t->pos.x - cam_t->pos.x) * cam->scale.x,
                        (cam_t->pos.y - t->pos.y) * cam->scale.y
                    };
                    screen_pos += screen_size * 0.5f;

                    int sprite_width = spr->image.width * t->scale.x * cam->scale.x;
                    int sprite_height = spr->image.height * t->scale.y * cam->scale.y;
                    screen_pos -= Vector2 {
                        (float) sprite_width,
                        (float) sprite_height
                    } * 0.5f;

                    if (sprite_width != spr->texture.width
                        || sprite_height != spr->texture.height)
                    {
                        UnloadTexture(spr->texture);
                        Image image = ImageCopy(spr->image);
                        ImageResizeNN(&image, sprite_width, sprite_height);
                        spr->texture = LoadTextureFromImage(image);
                    }

                    DrawTextureV(spr->texture, screen_pos, WHITE);
                }
            }
        }

        DrawFPS(10, 10);
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}
