#pragma once
#include <vector>
#include <string>
#include <typeinfo>
#include <iostream>
#include <typeindex>
#include <raylib-ext.hpp>
#include <box2d/box2d.h>

namespace plat {

typedef int Entity_id;

const float METERS_TO_PIXELS = 100;

class Storage;

class Component 
{
public:
    virtual void update(float dt, Entity_id parent_id, Storage &storage) = 0;
    virtual std::string get_component_type() = 0;
   
};

class Entity 
{
public:
    Entity_id id;
    std::vector<Component *> components;

    Entity();

    template <typename T> 
    T *
    getComponent()
    {
        for (int i = 0; i < components.size(); i++)
        {
            if (std::type_index(typeid(T)).name() == components[i]->get_component_type())
            {
                return (T *) components[i];
            }
        }

        return nullptr;
    }
};

class Storage
{
public:
    std::vector<Entity> entities;
    plat::Entity_id cur_camera;
    plat::Entity_id cur_world;
};

class Sprite : public Component 
{
public:
    Texture2D texture;
    Image image;

    Sprite(const std::string& path);
    std::string get_component_type() override;
    void update(float dt, Entity_id parent_id, Storage &storage) override;
};

class Transform : public Component 
{
public:
    Vector3 pos;
    Vector2 scale;
    float angle;

    void update(float dt, Entity_id parent_id, Storage &storage) override;
    std::string get_component_type() override;
};

class Player_control : public Component 
{
public:
    float speed;
    bool is_flying = false;
    bool is_waiting = true;
    bool is_jumping = false;
    bool is_falling = false;
    bool is_right = true;
    void update(float dt, Entity_id parent_id, Storage &storage) override;
    std::string get_component_type() override;
};

class Camera : public Component
{
public:
    Vector2 scale = {1, 1};

    void update(float dt, Entity_id parent_id, Storage &storage) override;
    std::string get_component_type() override;
};

class Physics : public Component
{
public:
    b2BodyDef bodyDef;
    b2Body * body;
    Rectangle collider;
    b2Body * contact;
    void update(float dt, Entity_id parent_id, Storage &storage) override;
    std::string get_component_type() override;
};

class World : public Component
{
public:
    b2World *cur_world;
    b2TimeStep time_settings;

    World(b2Vec2 gravity, float timestep = 1.0f/60.0f, int32 vel_it = 6, int32 pos_it = 2);
    void update(float dt, Entity_id parent_id, Storage &storage) override;
    std::string get_component_type() override;
};
} // namespace plat

/*
class _GUI : public Component
{
public:
    void Update(float dt, Entity_id parent_id, Storage& storage) override;
    std::string get_component_type();
};

class _Label : public Component
{
public:
    void Update(float dt, Entity_id parent_id, Storage& storage) override;
    std::string get_component_type();
    std::string text;
    Color modulate;
    int font_size;
};
*/
