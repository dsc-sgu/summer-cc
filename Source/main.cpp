#define RAYEXT_IMPLEMENTATION
#include <raylib-ext.hpp>
//#include "components.h"
#include "json_loader.h"
#include "render.h"
#include <iostream>
#include<algorithm>
Storage storage;
struct {
    bool operator()(Entity * a, Entity * b) const { 
        int x = a->getComponent<_Transform>()->pos.z;
        int y = b->getComponent<_Transform>()->pos.z;
        return x < y; 
    }
} customLess;


void z_order( std::vector<Entity*> entt, bool ord = true)
{
        
    if (ord){
        std::sort(entt.begin(), entt.end(), customLess);
    } else {
         std::sort(entt.begin(),entt.end(), customLess);
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

    storage = load_lvl("Asssets/Lvls/test.json");

    storage.entities.push_back(Entity());
    _Transform *trnsf1 = new _Transform();
    _Sprite *spr1 = new _Sprite("Asssets/Textures/bg.png");
    storage.entities[1].components.push_back(trnsf1);
    storage.entities[1].components.push_back(spr1);

    
    std::vector<Entity*> ent;

    for (int i = 0; i<storage.entities.size(); i++)
        ent.push_back(&(storage.entities[i]));
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(BLACK);
            z_order(ent);
            for (int i = 0; i < ent.size(); i++){
                for(int j = 0; j < ent[i]->components.size(); j++){
                    
                    ent[i]->components[j]->Update(GetFrameTime(), storage.entities[i].id, storage);
                    
                    _Transform* transform = ent[i]->getComponent<_Transform>();
                    _Sprite* sprite = ent[i]->getComponent<_Sprite>();
                    
                    Vector2 screen_transform = screen_to_local(viewport, screen_size, { transform->pos.x, transform->pos.y });

                    DrawTexture(sprite->texture, screen_transform.x, screen_transform.y-sprite->texture.height, WHITE);
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

