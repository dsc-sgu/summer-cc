#define RAYEXT_IMPLEMENTATION
#include <raylib-ext.hpp>
#include "components.h"
#include <iostream>

#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

Storage load_lvl(std::string path) {
    Storage new_lvl;
    json lvl_json;
    std::ifstream lvl_file(path);
    lvl_file >> lvl_json;
    int n = lvl_json["count"];

    

    for (int i = 0; i < n; i++) {
        new_lvl.entitys.push_back(Entity());

        auto entity = lvl_json["entities"][i];
        for(int j = 0; j<3; j++)
        if (entity[j]["type"] == "Transform") {
            _Transform* transform = new _Transform();
            transform->angle = entity[j]["angle"];
            transform->pos = Vector3{ entity[j]["pos"]["x"], entity[j]["pos"]["y"], entity[j]["pos"]["z"] };
            new_lvl.entitys[i].components.push_back(transform);
        }
        else if (entity[j]["type"] == "PlayerControl") {
            _Player_control* player_control = new _Player_control();
            player_control->speed = entity[j]["speed"];
            new_lvl.entitys[i].components.push_back(player_control);
        }
        else if (entity[j]["type"] == "Sprite") {
            _Sprite* sprite = new _Sprite(entity[j]["path"]);
            new_lvl.entitys[i].components.push_back(sprite);
        }
    }


    lvl_file.close();

    return new_lvl;
}

Storage storage;

int main()
{
   
    const int screenWidth = 640;
    const int screenHeight = 480;
    InitWindow(screenWidth, screenHeight, "GDSC: Creative Coding");
    SetTargetFPS(60);

    /*storage.entitys.push_back(Entity());
    _Player_control *player_control = new _Player_control();
    _Transform *trnsf = new _Transform();
    _Sprite *spr = new _Sprite("Asssets/Textures/King_stay.png");
    player_control->speed = 100;
    storage.entitys[0].components.push_back(player_control);
    storage.entitys[0].components.push_back(trnsf);
    storage.entitys[0].components.push_back(spr);*/

    storage = load_lvl("Asssets/Lvls/test.json");
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(BLACK);

            for(int i = 0; i<storage.entitys.size(); i++){
                for(int j = 0; j<storage.entitys[i].components.size(); j++){
                    std::cout << i << std::endl;
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

