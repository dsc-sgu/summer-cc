#include "components.h"

typedef int Entity_id;

Entity::Entity()
{
    static unsigned count = 0;
    this->id = count++;
}

std::string _Player_control::get_component_type()
{
    return std::type_index(typeid(_Player_control)).name();
}

std::string _Transform::get_component_type()
{
    return std::type_index(typeid(_Transform)).name();
}


void _Transform::Update(float dt, Entity_id parent_id, Storage &storage)
{}

void _Sprite::Update(float dt, Entity_id parent_id, Storage &storage)
{}

std::string _Sprite::get_component_type()
{
    return std::type_index(typeid(_Sprite)).name();
}

_Sprite::_Sprite(const std::string &path)
{
    image = LoadImage(path);
    texture = LoadTextureFromImage(image);
}

void _Player_control::Update(float dt, int parent_id, Storage &storage)
{
    auto cur_transform = storage.entities[parent_id].getComponent<_Transform>();
    
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
