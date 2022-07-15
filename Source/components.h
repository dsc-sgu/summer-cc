#include<vector>
#include<string>
#include<typeinfo>
#include<iostream>
#include<typeindex>
#include <raylib-ext.hpp>
typedef int Entity_id;

class Storage;

class Component 
{
public:

    virtual void Update(float dt, int parent_id, Storage &storage) = 0;
    virtual std::string get_component_type() = 0;
};

class Entity 
{
public:

    Entity_id id;
    std::vector<Component*> components;
    template <typename T> 
    T* getComponent()
    {

        for (int i = 0; i < components.size(); i++) {
            
            if(std::type_index(typeid(T)).name() == components[i]->get_component_type()) {
                return (T*) components[i];
            } 
        }

        return nullptr;
    }
};

class Storage
{
public:

    std::vector<Entity>entitys;
};

class _Sprite : public Component 
{
public:

    void Update(float dt, int parent_id, Storage &storage) override;
    std::string path; 
};

class _Transform : public Component 
{
public:

    void Update(float dt, int parent_id, Storage &storage) override;
    std::string get_component_type();
    Vector3 pos;
    float angle; 
};

class _Player_control : public Component 
{
public:

    void Update(float dt, int parent_id, Storage &storage) override;
    std::string get_component_type();
    int speed;
};
