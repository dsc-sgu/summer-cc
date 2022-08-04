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

    plat::Sprite *spr = storage.entities[parent_id].getComponent<Sprite>();
    if (!spr)
    {
        return;
    }

    b2Vec2 new_pos = ph->body->GetPosition();
    new_pos += {
        spr->image.width / 2.f,
        spr->image.height / 2.f
    };
    new_pos -= {
        ph->collider.x,
        ph->collider.y
    };
    new_pos -= {
        ph->collider.width / 2.f,
        ph->collider.height / 2.f
    };
    this->pos = Vector3 {
        new_pos.x,
        new_pos.y,
        this->pos.z
    };

    this->angle = - ph->body->GetAngle() * RAD2DEG;
    std::cout << "ANGLE: " << angle << '\n';
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
    auto cur_physics = storage.entities[parent_id].getComponent<Physics>();
    auto cur_world = storage.entities[storage.cur_world].getComponent<World>();
    bool isPressed[4]= {false};
   
    //std::cout << velocity.x << '\n';

    b2Vec2 velocity = cur_physics->body->GetLinearVelocity();
    // std::cout << velocity.x << ' ' << velocity.y << ' '<< "Velocity" << '\n';


    if (is_flying && is_jumping && velocity.y < 0.1)
    {
        is_jumping = false;
        is_falling = true;
        isPressed[0] = false;
    }
    else if (is_flying && is_falling && velocity.y > -0.1)
    {
        is_flying = false;
        is_falling = false;
        is_waiting = true;
    }

    if(IsKeyPressed(KEY_LEFT_SHIFT))
        cur_physics->body->SetGravityScale(0.1f);
    if(IsKeyReleased(KEY_LEFT_SHIFT))
        cur_physics->body->SetGravityScale(1.0f);
    if (IsKeyDown(KEY_SPACE) && is_waiting)
    {
        is_flying = true;
        is_waiting = false;
        is_jumping = true;
        isPressed[0] = true;
        // cur_physics->body->ApplyLinearImpulseToCenter(
            // b2Vec2(0, cur_physics->body->GetMass() * 5000 * dt),
            // true
        // );
    }
    if (IsKeyDown(KEY_S))
    {
        isPressed[1] = true;
        // cur_physics->body->ApplyLinearImpulseToCenter(
            // b2Vec2(0, -cur_physics->body->GetMass() * dt),
            // true
        // );
    }
    if (IsKeyDown(KEY_A))
    {
        isPressed[2] = true;
        // cur_physics->body->ApplyLinearImpulseToCenter(
            // b2Vec2(-cur_physics->body->GetMass() * speed * dt, 0),
            // true
        // );
  
        // if (is_right && velocity.x < - 0.1 )
        // {
        //     is_right = false;
        //     UnloadTexture(cur_sprite->texture);
        //     ImageFlipHorizontal(&cur_sprite->image);
        //     Image image = ImageCopy(cur_sprite->image);
        //     cur_sprite->texture = LoadTextureFromImage(image);
        // }
    }
    else if(IsKeyReleased(KEY_A))
    {
        cur_physics->body->SetLinearVelocity({0, cur_physics->body->GetLinearVelocity().y});
    }
    else if(IsKeyReleased(KEY_D))
    {
        cur_physics->body->SetLinearVelocity({0, cur_physics->body->GetLinearVelocity().y});
    }
    if (IsKeyDown(KEY_D))
    {
        isPressed[3] = true;
        //cur_physics->body->ApplyLinearImpulseToCenter(
          //  b2Vec2(cur_physics->body->GetMass() * speed * dt, 0),
            //true
        //);
        
        // if (!is_right && velocity.x >= 0 )
        // {
        //     is_right = true;
        //     UnloadTexture(cur_sprite->texture);
        //     ImageFlipHorizontal(&cur_sprite->image);
        //     Image image = ImageCopy(cur_sprite->image);
        //     cur_sprite->texture = LoadTextureFromImage(image);
        // }
    }
    else if(IsKeyReleased(KEY_D))
    {
        cur_physics->body->SetLinearVelocity({0, cur_physics->body->GetLinearVelocity().y});
    }
    // 
    // 
    b2Vec2 old_movement = cur_physics->body->GetLinearVelocity();
    b2Vec2 movement = {0, 0};
    if(isPressed[2] > isPressed[3])
        movement.x = std::min(old_movement.x, -float(speed))*1.5f;
     else if(isPressed[2] < isPressed[3])
        movement.x = std::max(old_movement.x, float(speed))*1.2f;
    if(isPressed[0] > isPressed[1])
        movement.y = std::max(float(speed) * speed , old_movement.y - float(speed) * speed)*32.81f;
       // movement.y = float(speed) * speed * 9.81f;
     else if (isPressed[0] < isPressed[1])
        movement.y -= float(speed)*1.2f ;
    //movement.x*=cur_physics->body->GetMass();
    if(is_waiting) {}
    else if(is_jumping)
         movement.y = std::max(movement.y, old_movement.y - cur_physics->body->GetMass()* 13.33f);
    else if (is_falling)
         movement.y = std::min(movement.y, old_movement.y -cur_physics->body->GetMass()*13.33f);
    //else //if (is_waiting && is_flying)
      //  movement.y = std::min(movement.y - cur_physics->body->GetMass()*9.81f, old_movement.y);
    // std::cout << movement.x << ' ' << movement.y <<' '<< cur_physics->body->GetMass() << '\n'

    cur_physics->body->ApplyLinearImpulseToCenter(movement, true);
    
    

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
    time_settings.velocityIterations = vel_it;
    time_settings.positionIterations = pos_it;
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
    cur_world->Step(dt, time_settings.velocityIterations, time_settings.positionIterations);
}

}
