#pragma once
#include <unordered_map>
#include "components.h"

namespace plat
{
    template<class Key, class T, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>, class Allocator = std::allocator< std::pair<const Key, T> >> class unordered_map;

    struct anim
    {
        std::string path;
        int frames; // количество кадров в анимации
    };

    class Animation_control : public Component
    {
    public:
        std::unordered_map <std::string, anim> anims;

        Animation_control(const std::string& path_stay, const int& frame_stay, const std::string& path_run, const int& frame_run, const std::string& path_attack, const int& frame_attack, const std::string path_hit, const int& frame_hit, const std::string path_dead, const int& frame_dead);

        std::string get_component_type() override;

        void update(float dt, Entity_id parent_id, Storage &storage) override;
    };

    class Animation : public Component 
    {
    public:
        Image base_image;

        Animation(const std::string& path);

        std::string get_component_type() override;
        void update(float dt, Entity_id parent_id, Storage &storage) override;
    };
}