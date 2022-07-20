#pragma once
#include <vector>
#include <string>
#include <typeinfo>
#include <iostream>
#include <typeindex>
#include <raylib-ext.hpp>

typedef int Entity_id;

class Storage;

class Component 
{
public:
    virtual void Update(float dt, Entity_id parent_id, Storage &storage) = 0;
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
};

class _Sprite : public Component 
{
public:
    Texture2D texture;
    Image image;

    _Sprite(const std::string& path);
    _Sprite();
    void Update(float dt, Entity_id parent_id, Storage &storage) override;
    std::string get_component_type() override;
};

class _Transform : public Component 
{
public:
    Vector3 pos;
    float scale;
    float angle; 

    void Update(float dt, Entity_id parent_id, Storage &storage) override;
    std::string get_component_type() override;
};

class _Player_control : public Component 
{
public:
    int speed;

    void Update(float dt, Entity_id parent_id, Storage &storage) override;
    std::string get_component_type() override;
};
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