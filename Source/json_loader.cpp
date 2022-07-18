#include "json_loader.h"

using json = nlohmann::json;

Storage load_lvl(std::string path) {
    Storage new_lvl;
    json lvl_json;
    std::ifstream lvl_file(path);
    lvl_file >> lvl_json;
    int n = lvl_json["count"];

    for (int i = 0; i < n; i++) {
        new_lvl.entities.push_back(Entity());
        new_lvl.entities[i].id = i;
        auto entity = lvl_json["entities"][i];
        for (int j = 0; j < 3; j++)
            if (entity[j]["type"] == "Transform") {
                _Transform* transform = new _Transform();
                transform->angle = entity[j]["angle"];
                transform->pos = Vector3{ entity[j]["pos"]["x"], entity[j]["pos"]["y"], entity[j]["pos"]["z"] };
                new_lvl.entities[i].components.push_back(transform);
            }
            else if (entity[j]["type"] == "PlayerControl") {
                _Player_control* player_control = new _Player_control();
                player_control->speed = entity[j]["speed"];
                new_lvl.entities[i].components.push_back(player_control);
            }
            else if (entity[j]["type"] == "Sprite") {
                _Sprite* sprite = new _Sprite(entity[j]["path"]);
                new_lvl.entities[i].components.push_back(sprite);
            }
    }

    lvl_file.close();

    return new_lvl;
}
