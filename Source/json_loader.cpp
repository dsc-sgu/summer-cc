#include "json_loader.h"

using json = nlohmann::json;

plat::Storage
load_lvl(std::string path)
{
    plat::Storage new_lvl;
    std::ifstream lvl_file(path);
    json lvl_json;
    lvl_file >> lvl_json;

    for (auto &entity : lvl_json["entities"])
    {
        new_lvl.entities.push_back(plat::Entity());
        for (auto &component : entity)
        {
            if (component["type"] == "Transform")
            {
                plat::Transform *transform = new plat::Transform();
                transform->angle = component["angle"];
                transform->pos = Vector3 {
                    component["pos"][0],
                    component["pos"][1],
                    component["pos"][2]
                };
                transform->scale = Vector2 {
                    component["scale"][0],
                    component["scale"][1]
                };
                new_lvl.entities.back().components.push_back(transform);
            }
            else if (component["type"] == "PlayerControl")
            {
                plat::Player_control *player_control = new plat::Player_control();
                player_control->speed = component["speed"];
                new_lvl.entities.back().components.push_back(player_control);
            }
            else if (component["type"] == "Sprite")
            {
                new_lvl.entities.back().components.push_back(
                    new plat::Sprite(std::string(component["path"]))
                );
            }
            else if (component["type"] == "Camera")
            {
                plat::Camera *cam = new plat::Camera();
                cam->scale = Vector2 {
                    component["scale"][0],
                    component["scale"][1]
                };
                new_lvl.entities.back().components.push_back(cam);
                new_lvl.cur_camera = new_lvl.entities.size() - 1;
            }
        }
    }

    lvl_file.close();

    return new_lvl;
}
