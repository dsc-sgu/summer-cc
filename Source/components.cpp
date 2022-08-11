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
    new_pos.x *= plat::METERS_TO_PIXELS;
    new_pos.y *= plat::METERS_TO_PIXELS;

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
    b2Vec2 velocity = cur_physics->body->GetLinearVelocity();

    if (is_flying && is_jumping && velocity.y < -0.1)
    {
        is_jumping = false;
        is_falling = true;
    }
    else if (is_flying && is_falling && velocity.y > -0.05)
    {
        is_flying = false;
        is_falling = false;
        is_waiting = true;
    }
    if (IsKeyDown(KEY_RIGHT_SHIFT))
        cur_physics->body->SetGravityScale(0.1f);
    else
        cur_physics->body->SetGravityScale(1.f);

    if (storage.axes["jump"] && is_waiting)
    {
        is_flying = true;
        is_waiting = false;
        is_jumping = true;
        cur_physics->body->ApplyLinearImpulseToCenter(
            b2Vec2(0, 350),
            true
        );
    }

    is_right = storage.axes["horizontal"] < 0;
    const b2Vec2 &pos = cur_physics->body->GetPosition();
    const float angle = cur_physics->body->GetAngle();
    b2Vec2 newpos = pos;
    newpos.x += speed * dt * storage.axes["horizontal"];
    cur_physics->body->SetTransform(newpos, angle);
    cur_physics->body->SetAwake(true);

    if (storage.axes["kick"] > 0 && is_waiting)
    {
        b2Body *cntct = cur_physics->body->GetContactList()->contact->GetFixtureB()->GetBody();
        if (cntct->GetMass()!=0)
            cur_physics->contact = cntct;
        if (cur_physics->contact->GetType() == b2_dynamicBody &&
            std::abs(cur_physics->contact->GetPosition().x - cur_physics->body->GetPosition().x) < 0.5f)
        {
            // std::cout << cntct->GetPosition().x<<"\n";
            float dir = -1.f;
            if(is_right)
                dir = 1.f;
            b2Vec2 movement = {100.f, 250.f};
            movement.x *= dir;
            cur_physics->contact->ApplyLinearImpulseToCenter(movement, true);
            std::cout << cntct->GetLinearVelocity().x << " Mass: " << cntct->GetMass() << " Position: "<<cntct->GetPosition().y <<'\n';
        }
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

#if 1
    Vector2 cam_speed = Vector2 {
        storage.axes["cam_x"],
        storage.axes["cam_y"],
    } * dt;

    t->pos.x += cam_speed.x;
    t->pos.y -= cam_speed.y;
#endif
#if 0
    Vector2 cam_speed = Vector2 {
        1, 1
    } * dt;

    if (IsKeyDown(KEY_LEFT))
    {
        t->pos.x -= cam_speed.x;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        t->pos.x += cam_speed.x;
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
            t->pos.y += cam_speed.y;
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
            t->pos.y -= cam_speed.y;
        }
    }
#endif
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
