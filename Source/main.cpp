#define RAYEXT_IMPLEMENTATION
#include <raylib-ext.hpp>
#include <iostream>
#include <algorithm>
#include "json_loader.h"
#include "render.h"

void
updateAxes(plat::Storage &storage)
{

    if (IsGamepadAvailable(0))
        storage.axes["horizontal"] =
            GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
    else if (IsKeyDown(KEY_D))
        storage.axes["horizontal"] = 1.0f;
    else if (IsKeyDown(KEY_A))
        storage.axes["horizontal"] = -1.0f;
    else
        storage.axes["horizontal"] = 0;

    if (storage.axes["jump"] > 0)
        storage.axes["jump"] = 0;
    if (IsKeyDown(KEY_SPACE))
        storage.axes["jump"] = 1;
}

int
main()
{
    const Vector2 screen_size { 720, 480 };
    InitWindow(screen_size.x, screen_size.y, "Creative Coding: Platformer");
    SetTargetFPS(60);

    plat::Storage storage = load_lvl("Assets/Scenes/default.json");
    storage.axes["horizontal"] = 0;
    storage.axes["jump"] = 0;
    std::vector<plat::Entity *> draw_queue = create_draw_order(storage.entities);
    
    while (!WindowShouldClose())
    {
        updateAxes(storage);
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
                    plat::Physics *ph = draw_queue[i]->getComponent<plat::Physics>();

                    Vector2 screen_pos = {
                        (t->pos.x - cam_t->pos.x * plat::METERS_TO_PIXELS) * cam->scale.x,
                        (cam_t->pos.y * plat::METERS_TO_PIXELS - t->pos.y) * cam->scale.y
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

                    // NOTE (mchernigin): Отрисовка коллайдеров
                    if (ph)
                    {
                        auto c = ph->body->GetFixtureList()->GetAABB(0).GetCenter();
                        auto a = ph->body->GetFixtureList()->GetAABB(0).lowerBound;
                        auto b = ph->body->GetFixtureList()->GetAABB(0).upperBound;
                        c *= plat::METERS_TO_PIXELS;
                        a *= plat::METERS_TO_PIXELS;
                        b *= plat::METERS_TO_PIXELS;
                        Vector2 screen_pos = Vector2 {
                            (c.x - cam_t->pos.x * plat::METERS_TO_PIXELS) * cam->scale.x,
                            (cam_t->pos.y * plat::METERS_TO_PIXELS - c.y) * cam->scale.y
                        };
                        screen_pos += screen_size * 0.5f;
                        screen_pos -= Vector2 {
                            (float) (b.x - a.x) * cam->scale.x,
                            (float) (b.y - a.y) * cam->scale.y
                        } * 0.5f;
                        DrawRectangleLines(
                            screen_pos.x,
                            screen_pos.y,
                            (b.x - a.x + 1) * cam->scale.x,
                            (b.y - a.y + 1) * cam->scale.y,
                            RED
                        );
                    }
                }
            }
        }

        DrawFPS(10, 10);
        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}
