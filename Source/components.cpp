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
    auto cur_sprite = storage.entities[parent_id].getComponent<Sprite>();
    float impulse = cur_physics->body->GetMass()* speed;
    b2Vec2 velocity = cur_physics->body->GetLinearVelocity();

    if (!is_waiting && velocity.y < 0)
        is_waiting = true;
    else if (is_waiting && velocity.y > -0.5f)
        is_flying = false;
    
    if(IsKeyPressed(KEY_LEFT_SHIFT))
        cur_physics->body->SetGravityScale(0.1f);
     if(IsKeyReleased(KEY_LEFT_SHIFT))
        cur_physics->body->SetGravityScale(1.0f);
    if (IsKeyDown(KEY_W))
    {
        if (!is_flying && cur_physics->body->GetLinearVelocity().y > -0.1)
        {
            is_flying = true;
            is_waiting = false;
            // DON'T TOUCH NUMBER, IT'S MAGICAL(you can increase it, but don't decrease it)
            //impulse *= 50;
            cur_physics->body->ApplyLinearImpulse(
                b2Vec2(velocity.x, impulse),
                cur_physics->body->GetWorldCenter(), true
            );
        }
    }
    if (IsKeyDown(KEY_S))
    {
        impulse *= -50;
        cur_physics->body->ApplyLinearImpulse(
            b2Vec2(velocity.x * cur_physics->body->GetMass(), impulse),
            cur_physics->body->GetWorldCenter(), true
        );
    }
    if (IsKeyDown(KEY_A))
    {
        // DON'T TOUCH NUMBER, IT'S MAGICAL(you can increase it, but don't decrease it)
        impulse /= 100;
        cur_physics->body->ApplyLinearImpulse(
            b2Vec2(-impulse, 0),
            cur_physics->body->GetWorldCenter(), true
        );
        
        if (is_right && velocity.x < - 0.1 )
        {
            is_right = false;
            UnloadTexture(cur_sprite->texture);
            ImageFlipHorizontal(&cur_sprite->image);
            Image image = ImageCopy(cur_sprite->image);
            cur_sprite->texture = LoadTextureFromImage(image);
        }
    }
    else if (IsKeyReleased(KEY_A))
    {
        velocity.x = 0.f;
        cur_physics->body->SetLinearVelocity(velocity);
        cur_physics->body->SetAngularVelocity(0);
    }
    if (IsKeyDown(KEY_D))
    {
        // DON'T TOUCH NUMBER, IT'S MAGICAL(you can increase it, but don't decrease it)
        impulse /= 100;
        cur_physics->body->ApplyLinearImpulse(
            b2Vec2(impulse, 0),
            cur_physics->body->GetWorldCenter(), true
        );
        
        if (!is_right && velocity.x >= 0 )
        {
            is_right = true;
            UnloadTexture(cur_sprite->texture);
            ImageFlipHorizontal(&cur_sprite->image);
            Image image = ImageCopy(cur_sprite->image);
            cur_sprite->texture = LoadTextureFromImage(image);
        }
    }
    else if (IsKeyReleased(KEY_D))
    {
        velocity.x = 0.f;
        cur_physics->body->SetLinearVelocity(velocity);
        cur_physics->body->SetAngularVelocity(0);
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
