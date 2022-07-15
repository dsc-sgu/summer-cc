#define RAYEXT_IMPLEMENTATION
#include <raylib-ext.hpp>
#include "components.h"
#include <iostream>

Storage storage;

int main()
{

    const int screenWidth = 640;
    const int screenHeight = 480;
    InitWindow(screenWidth, screenHeight, "GDSC: Creative Coding");
    SetTargetFPS(60);

    storage.entitys.push_back(Entity());
    _Player_control *player_control = new _Player_control();
    _Transform *trnsf = new _Transform();
    _Sprite *spr = new _Sprite("Asssets/Textures/King_stay.png");
    player_control->speed = 100;
    storage.entitys[0].components.push_back(player_control);
    storage.entitys[0].components.push_back(trnsf);
    storage.entitys[0].components.push_back(spr);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(BLACK);

            for(int i = 0; i<storage.entitys.size(); i++){
                for(int j = 0; j<storage.entitys[i].components.size(); j++){

                    storage.entitys[i].components[j]->Update(GetFrameTime(),storage.entitys[i].id,storage);
                    auto t = storage.entitys[i].getComponent<_Transform>();
                    auto s = storage.entitys[i].getComponent<_Sprite>();
                    DrawTexture(s->texture, t->pos.x, t->pos.y, WHITE);
                }   
            }
        }

        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}

