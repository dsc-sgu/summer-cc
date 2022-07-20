#include "json_loader.h"

using json = nlohmann::json;

Storage
load_lvl(std::string path)
{
    Storage new_lvl;
    std::ifstream lvl_file(path);
    json lvl_json;
    lvl_file >> lvl_json;

    for (auto &entity : lvl_json["entities"])
    {
        new_lvl.entities.push_back(Entity());
        for (auto &component : entity)
        {
            if (component["type"] == "Transform")
            {
                _Transform *transform = new _Transform();
                transform->angle = component["angle"];
                transform->pos = (Vector3) {
                    component["pos"][0],
                    component["pos"][1],
                    component["pos"][2]
                };
                new_lvl.entities.back().components.push_back(transform);
            }
            else if (component["type"] == "PlayerControl")
            {
                _Player_control *player_control = new _Player_control();
                player_control->speed = component["speed"];
                new_lvl.entities.back().components.push_back(player_control);
            }
            else if (component["type"] == "Sprite")
            {
                new_lvl.entities.back().components.push_back(
                    new _Sprite(std::string(component["path"]))
                );
            }
        }
    }

    lvl_file.close();

    return new_lvl;
}
