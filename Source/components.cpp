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
Transform::update(float dt, Entity_id parent_id, Storage &storage)
{
    plat::Physics *ph = storage.entities[parent_id].getComponent<Physics>();
    if (!ph)
    {
        return;
    }

    b2Vec2 new_pos = ph->body->GetPosition();
    this->pos = Vector3 { new_pos.x, new_pos.y, this->pos.z };
}

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
Sprite::update(float dt, Entity_id parent_id, Storage &storage)
{}

std::string
Player_control::get_component_type()
{
    return std::type_index(typeid(Player_control)).name();
}

void
Player_control::update(float dt, int parent_id, Storage &storage)
{
    auto cur_transform = storage.entities[parent_id].getComponent<Transform>();
    auto cur_physics = storage.entities[parent_id].getComponent<Physics>();
    b2Vec2 velocity = cur_physics->body->GetLinearVelocity();
    if (!is_waiting && velocity.y < 0)
    {
        is_waiting = true;
        cur_physics->body->SetGravityScale(1.0f);
    }
    else if (is_waiting && velocity.y > -0.5f)
        {
        is_flying = false;
        cur_physics->body->SetGravityScale(1.0f);
        }
    if (IsKeyDown(KEY_W))
    {
        //cur_physics->bodyDef.position.y +=speed * dt;
        //cur_transform->pos.y += speed * dt;
        //cur_physics->body->SetAngularVelocity(100);
        //b2Vec2 velocity = cur_physics->body->GetLinearVelocity();    
        //cur_physics->body->SetLinearVelocity(velocity);
        if (!is_flying)
        {
        is_flying = true;
        is_waiting = false;
        velocity.y+=250.f* speed * dt;
        cur_physics->body->SetGravityScale(0.1f);
        cur_physics->body->SetLinearVelocity(velocity);
        }
    }
    if (IsKeyDown(KEY_S))
    {
        cur_physics->bodyDef.position.y -=speed * dt;
        cur_transform->pos.y -= speed * dt;
    }
    if (IsKeyDown(KEY_A))
    {
        velocity.x -= speed * dt;
        cur_physics->body->SetLinearVelocity(velocity);
        //cur_physics->bodyDef.position.x -=speed * dt;
        //cur_transform->pos.x -= speed * dt;
    }
    else if (IsKeyReleased(KEY_A))
    {
        velocity.x = 0.f;
        cur_physics->body->SetLinearVelocity(velocity);
    }
    if (IsKeyDown(KEY_D))
    {
        velocity.x += speed * dt;
        cur_physics->body->SetLinearVelocity(velocity);
        //cur_physics->bodyDef.position.x +=speed * dt;
        //cur_transform->pos.x += speed * dt;
    }
    else if (IsKeyReleased(KEY_D))
    {
        velocity.x = 0.f;
        cur_physics->body->SetLinearVelocity(velocity);
    }
}

std::string
Camera::get_component_type()
{
    return std::type_index(typeid(Camera)).name();
}

void
Camera::update(float dt, Entity_id parent_id, Storage &storage)
{
    plat::Transform *t = storage.entities[parent_id].getComponent<Transform>();
    plat::Camera *cam = storage.entities[parent_id].getComponent<Camera>();
    if (IsKeyDown(KEY_LEFT))
    {
        t->pos.x -= 1;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        t->pos.x += 1;
    }
    if (IsKeyDown(KEY_UP))
    {
        if (IsKeyDown(KEY_LEFT_SHIFT))
        {
            cam->scale.x *= 1.25;
            cam->scale.y *= 1.25;
        }
        else
        {
            t->pos.y += 1;
        }
    }
    if (IsKeyDown(KEY_DOWN))
    {
        if (IsKeyDown(KEY_LEFT_SHIFT))
        {
            cam->scale.x *= 1 / 1.25;
            cam->scale.y *= 1 / 1.25;
        }
        else
        {
            t->pos.y -= 1;
        }
    }
}

std::string
Physics::get_component_type()
{
    return std::type_index(typeid(Physics)).name();
}

void
Physics::update(float dt, Entity_id parent_id, Storage &storage)
{}

World::World(b2Vec2 gravity, float timestep, int32 vel_it, int32 pos_it)
{
    b2World *world = new b2World(gravity);
    time_settings.dt = timestep;
    time_settings.positionIterations = pos_it;
    time_settings.velocityIterations = vel_it;
    cur_world = world; 
}

std::string
World::get_component_type()
{
    return std::type_index(typeid(World)).name();
}

void
World::update(float dt, Entity_id parent_id, Storage &storage)
{
    cur_world->Step(time_settings.dt, time_settings.velocityIterations ,time_settings.positionIterations);
}

}
