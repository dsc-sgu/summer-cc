#include "components.h"

namespace plat
{
    Entity::Entity()
    {
        static Entity_id count = 0;
        id = count++;
    }

    Sprite::Sprite(const std::string &path)
    {
        image = LoadImage(path);
        texture = LoadTextureFromImage(image);
    }

    World::World(b2Vec2 gravity, float timestep, int32 vel_it, int32 pos_it)
    {
        b2World *world = new b2World(gravity);
        time_settings.dt = timestep;
        time_settings.velocityIterations = vel_it;
        time_settings.positionIterations = pos_it;
        cur_world = world; 
    }

//-----------------------------------------------------------------------------------------------

    std::string Transform::get_component_type()
    {
        return std::type_index(typeid(Transform)).name();
    }

    std::string Player_control::get_component_type()
    {
        return std::type_index(typeid(Player_control)).name();
    }

    std::string Sprite::get_component_type()
    {
        return std::type_index(typeid(Sprite)).name();
    }

    std::string Physics::get_component_type()
    {
        return std::type_index(typeid(Physics)).name();
    }

    std::string Camera::get_component_type()
    {
        return std::type_index(typeid(Camera)).name();
    }

    std::string World::get_component_type()
    {
        return std::type_index(typeid(World)).name();
    }

//-----------------------------------------------------------------------------------------------

    void Transform::update(float dt, Entity_id parent_id, Storage &storage)
    {
        plat::Physics *ph = storage.entities[parent_id].getComponent<Physics>();
        if (!ph) return;

        plat::Sprite *spr = storage.entities[parent_id].getComponent<Sprite>();
        if (!spr) return;

        b2Vec2 new_pos = ph->body->GetPosition();

        new_pos += {spr->image.width / 2.f, spr->image.height / 2.f};
        new_pos -= {ph->collider.x, ph->collider.y};
        new_pos -= {ph->collider.width / 2.f, ph->collider.height / 2.f};

        this->pos = Vector3 {new_pos.x, new_pos.y, this->pos.z};
    }

    void Player_control::update(float dt, int parent_id, Storage &storage)
    {
        auto cur_physics = storage.entities[parent_id].getComponent<Physics>();
        b2Vec2 velocity = cur_physics->body->GetLinearVelocity();

        if (!is_waiting && velocity.y < 0)
        {
            is_waiting = true;
        }
        /*else if (is_waiting && velocity.y > -0.5f)
        {
            is_flying = false;
        }*/

        if(IsKeyPressed(KEY_LEFT_SHIFT)) cur_physics->body->SetGravityScale(0.1f);
        if(IsKeyReleased(KEY_LEFT_SHIFT)) cur_physics->body->SetGravityScale(1.0f);
        /*if (IsKeyDown(KEY_SPACE) && !is_flying && cur_physics->body->GetLinearVelocity().y > -0.1)
        {
            is_flying = true;
            is_waiting = false;

            cur_physics->body->ApplyLinearImpulseToCenter(b2Vec2(0, cur_physics->body->GetMass() * 5000 * dt), true);
        }*/
        if (IsKeyDown(KEY_S)) cur_physics->body->ApplyLinearImpulseToCenter(b2Vec2(0, -cur_physics->body->GetMass() * dt), true);
        if (IsKeyDown(KEY_A))
        {
            cur_physics->body->ApplyLinearImpulseToCenter(b2Vec2(-cur_physics->body->GetMass() * speed * dt, 0), true);
    
            if (is_right && velocity.x < - 0.1 ) is_right = false;
        }
        if (IsKeyDown(KEY_D))
        {
            cur_physics->body->ApplyLinearImpulseToCenter(b2Vec2(cur_physics->body->GetMass() * speed * dt, 0), true);
            
            if (!is_right && velocity.x >= 0 ) is_right = true;
        }
    }

    void Sprite::update(float dt, Entity_id parent_id, Storage &storage)
    {}

    void Camera::update(float dt, Entity_id parent_id, Storage &storage)
    {
        plat::Transform *t = storage.entities[parent_id].getComponent<Transform>();
        plat::Camera *cam = storage.entities[parent_id].getComponent<Camera>();

        if (IsKeyDown(KEY_LEFT)) t->pos.x -= 1;
        if (IsKeyDown(KEY_RIGHT)) t->pos.x += 1;
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

    void World::update(float dt, Entity_id parent_id, Storage &storage)
    {
        cur_world->Step(dt, time_settings.velocityIterations, time_settings.positionIterations);
    }
//-----------------------------------------------------------------------------------------------
}
