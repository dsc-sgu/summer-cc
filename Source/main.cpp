#define RAYEXT_IMPLEMENTATION
#include <raylib-ext.hpp>
//#include "components.h"
#include "json_loader.h"
#include <iostream>




Storage storage;

int main()
{
   
    const int screenWidth = 640;
    const int screenHeight = 480;
    InitWindow(screenWidth, screenHeight, "GDSC: Creative Coding");
    SetTargetFPS(60);

    storage = load_lvl("Asssets/Lvls/test.json");
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(BLACK);

            for(int i = 0; i<storage.entitys.size(); i++){
                for(int j = 0; j<storage.entitys[i].components.size(); j++){
                    
                    storage.entitys[i].components[j]->Update(GetFrameTime(), storage.entitys[i].id, storage);
                    
                    _Transform* transform = storage.entitys[i].getComponent<_Transform>();
                    _Sprite* sprite = storage.entitys[i].getComponent<_Sprite>();
                    
                    DrawTexture(sprite->texture, transform->pos.x, transform->pos.y, WHITE);
                }   
            }
        }

        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}

