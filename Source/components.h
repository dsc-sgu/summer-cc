#pragma once
#include <vector>
#include <string>
#include <typeinfo>
#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <box2d/box2d.h>
#include <raylib-ext.hpp>

namespace plat
{
    typedef int Entity_id;

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

        template <typename T> T * getComponent()
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

    class Animation : public Component 
    {
    public:
        Image base_image; // картинка заданная по умолчанию?

        Animation(const std::string& path);
        std::string get_component_type() override;
        void update(float dt, Entity_id parent_id, Storage &storage) override;
    };

    class Animation_control : public Component
    {
    public:
        struct anim
        {
            std::string path; // Путь к текстурке
            int frames; // Количество кадров в анимации
            float anim_delay; // скорость проигрывания анимации
        };

        std::unordered_map <std::string, anim> anims; // массив для хранения анимаций

        std::string curent_animation = "STAY";

        Animation_control(
            const std::string& path_stay, const int& frame_stay, const float anim_delay_stay, 
            const std::string& path_run, const int& frame_run, const float anim_delay_run, 
            const std::string& path_attack, const int& frame_attack, const float anim_delay_attack,
            const std::string path_hit, const int& frame_hit, const float anim_delay_hit,
            const std::string path_dead, const int& frame_dead, const float anim_delay_dead);
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
        int speed; // скорость перемещения персонажа (задана в json)
        bool is_waiting = true; // поумолчанию персонаж ничего не делает и находится в режиме ожидания
        bool is_right = true; // поумолчанию пероснаж смотрит напрво (хотя это наверное лучше задать в json, тк на разных уровнях стартовая позиция может отличаться)
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
}