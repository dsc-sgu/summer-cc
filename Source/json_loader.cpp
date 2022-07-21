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
            if (component["type"] == "World")
            {
                b2Vec2 gravity(component["gravity"][0], component["gravity"][1]);
                float time = component["timeStep"];
                int32 velocityIterations = component["Velocity"];
                int32 positionIterations = component ["Position"];
                plat::World *Cur_World = new plat::World(
                    gravity,
                    time,
                    velocityIterations,
                    positionIterations);
                new_lvl.entities.back().components.push_back(Cur_World);
            }
            else if (component["type"] == "Transform")
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
            else if (component["type"] == "Physics")
            {
                if (component["type"] == "dynamic")
                {
                    plat::Physics * cur_phys = new plat::Physics();
                    cur_phys->bodyDef.type = b2_dynamicBody;
                    plat::Transform *trans = new_lvl.entities.back().getComponent<plat::Transform>();
                }
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
