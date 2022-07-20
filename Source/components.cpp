#include "components.h"

namespace plat {

Entity::Entity()
{
    static Entity_id count = 0;
    id = count++;
}

std::string
Transform::get_component_type()
{
    return std::type_index(typeid(Transform)).name();
}

void
Transform::Update(float dt, Entity_id parent_id, Storage &storage)
{}

Sprite::Sprite(const std::string &path)
{
    image = LoadImage(path);
    texture = LoadTextureFromImage(image);
}

std::string
Sprite::get_component_type()
{
    return std::type_index(typeid(Sprite)).name();
}

void
Sprite::Update(float dt, Entity_id parent_id, Storage &storage)
{}

std::string
Player_control::get_component_type()
{
    return std::type_index(typeid(Player_control)).name();
}

void
Player_control::Update(float dt, int parent_id, Storage &storage)
{
    auto cur_transform = storage.entities[parent_id].getComponent<Transform>();

    if (IsKeyDown(KEY_W))
    {
        cur_transform->pos.y += speed * dt;
    }
    else if (IsKeyDown(KEY_S))
    {
        cur_transform->pos.y -= speed * dt;
    }
    
    if (IsKeyDown(KEY_A))
    {
        cur_transform->pos.x -= speed * dt;
    }
    else if (IsKeyDown(KEY_D))
    {
        cur_transform->pos.x += speed * dt;
    }
}

}
