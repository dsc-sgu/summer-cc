#include<vector>
#include<string>
#include<typeinfo>
#include<iostream>
#include<typeindex>
typedef int Entity_id;

class Component {
public:
    virtual void Update(float dt) = 0;
    virtual std::string get_component_type() = 0;
};

class Entity {
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

class Storage{
public:
    std::vector<Entity>entitys;
};

class Player_control : public Component {
public:
    void Update(float dt) override;
    std::string get_component_type();
    int x, y;
    float speed;
};

class Sprite:Component {
    public:
        void Update(float dt) override;
        std::string path; 
};