#include"components.h"
#include"raylib.h"
#include<vector>
typedef int Entity_id;

std::string Player_control::get_component_type()
{
    return std::type_index(typeid(Player_control)).name();
}
void Player_control::Update(float dt)
{
    if (IsKeyDown(KEY_W))
    {
        this->y -= speed*dt;
    }
    else if (IsKeyDown(KEY_S))
    {
        this->y += speed*dt;
    }
    
    if (IsKeyDown(KEY_A))
    {
        this->x -= speed*dt;
    }
    else if (IsKeyDown(KEY_D))
    {
        this->x += speed*dt;
    }
}
