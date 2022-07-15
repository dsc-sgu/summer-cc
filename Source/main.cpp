#define RAYEXT_IMPLEMENTATION
#include <raylib-ext.hpp>
#include "components.h"
#include <iostream>

Storage storage;

int main()
{

    const int screenWidth = 640;
    const int screenHeight = 480;

    storage.entitys.push_back(Entity());
    _Player_control *player_control = new _Player_control();
    _Transform *trnsf = new _Transform();
    player_control->speed = 100;
    storage.entitys[0].components.push_back(player_control);
    storage.entitys[0].components.push_back(trnsf);
    InitWindow(screenWidth, screenHeight, "GDSC: Creative Coding");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(BLACK);

            for(int i = 0; i<storage.entitys.size(); i++){
                for(int j = 0; j<storage.entitys[i].components.size(); j++){

                    storage.entitys[i].components[j]->Update(GetFrameTime(),storage.entitys[i].id,storage);
                    auto t = storage.entitys[i].getComponent<_Transform>();
                    DrawRectangle(t->pos.x, t->pos.y, 10, 10, GREEN);
                }   
            }
        }

        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}

