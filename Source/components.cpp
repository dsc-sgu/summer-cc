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

    Animation::Animation(const std::string& path)
    {
        Image base_image = LoadImage(path); // скорее всего надо будет переделать
    }

    Animation_control::Animation_control(const std::string& path_stay, const int& frame_stay, const float anim_delay_stay, const std::string& path_run, const int& frame_run, const float anim_delay_run, const std::string& path_attack, const int& frame_attack, const float anim_delay_attack, const std::string path_hit, const int& frame_hit, const float anim_delay_hit, const std::string path_dead, const int& frame_dead, const float anim_delay_dead)
    {}

    World::World(b2Vec2 gravity, float timestep, int32 vel_it, int32 pos_it)
    {
        b2World *world = new b2World(gravity);
        time_settings.dt = timestep;
        time_settings.velocityIterations = vel_it;
        time_settings.positionIterations = pos_it;
        cur_world = world; 
    }

//-----------------------------------------------------------------------------------------------
// функции типа get_component_type()

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

    std::string Animation::get_component_type()
    {
        return std::type_index(typeid(Animation)).name();
    }

    std::string Animation_control::get_component_type()
    {
        return std::type_index(typeid(Animation_control)).name();
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
// функции типа update()

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

        // Если ничего не происходит(ничего не нажато), то персонаж ничего не делает(анимация поумолчанию)
        if (!is_waiting && velocity.y < 0)  is_waiting = true;

        // прыжок пока идёт нах
        /*if (IsKeyDown(KEY_SPACE) && !... && cur_physics->body->GetLinearVelocity().y > -0.1) // Если нажать пробел он прыгнет
        {
            is_waiting = false;

            cur_physics->body->ApplyLinearImpulseToCenter(b2Vec2(0, cur_physics->body->GetMass() * 5000 * dt), true);
        }*/

        // Если нажать A, то персонаж пойдёт налево)
        if (IsKeyDown(KEY_A))
        {
            cur_physics->body->ApplyLinearImpulseToCenter(b2Vec2(-cur_physics->body->GetMass() * speed * dt, 0), true);
    
            if (is_right && velocity.x < - 0.1 ) is_right = false;
        }
        // Если нажать D, то персонаж пойдёт направо
        if (IsKeyDown(KEY_D))
        {
            cur_physics->body->ApplyLinearImpulseToCenter(b2Vec2(cur_physics->body->GetMass() * speed * dt, 0), true);
            
            if (!is_right && velocity.x >= 0 ) is_right = true;
        }
    }

    void Sprite::update(float dt, Entity_id parent_id, Storage &storage)
    {}

    void Animation::update(float dt, Entity_id parent_id, Storage &storage)
    {
        plat::Sprite *spr = storage.entities[parent_id].getComponent<Sprite>();
        plat::Animation_control *anim_cntrl = storage.entities[parent_id].getComponent<Animation_control>();
        plat::Physics *phys = storage.entities[parent_id].getComponent<Physics>();
        plat::Transform *trns = storage.entities[parent_id].getComponent<Transform>();
        plat::Player_control *plr_cntrl = storage.entities[parent_id].getComponent<Player_control>();

        const int padding = 2 * (this->base_image.width / 2 - (phys->collider.x + phys->collider.width / 2)); // что это?

        spr->image = ImageCopy(this->base_image); //

        int frameCounter = 0; // счётчик кадров анимации


        // что это?
        // что оно делает?
        /*
        // Если персонаж находится в прыжке(полёте) и не смотрит направо, то текстурка инвертируется погоризонтали и... что-то ещё
        // прыжок всё ешё идёт нах
        if((plr_cntrl->is_flying) & (!plr_cntrl->is_right))
        {
            ImageFlipHorizontal(&spr->image);
            trns->pos.x -= padding;
        }
        else*/ 
        if(plr_cntrl->is_right) 
        {
            if (frameCounter >= anim_cntrl.anims[/*надо как-то получить имя анимации*/].anim_delay) // Если счётчик кадров больше чем кадров в анимации то цикл начинается заного
            {
                float x = 78 * ((frameCounter / anim_cntrl.anims[/*надо как-то получить имя анимации*/].frames) % anim_cntrl.anims[/*надо как-то получить имя анимации*/].frames);
                float y = 0;

                ImageCrop(&spr->image, {x, y, 78, 58});
            }

            ++frameCounter;
        }
        else if(!plr_cntrl->is_right) // Если персонаж не смотрит направо
        {
            if (frameCounter >= anim_cntrl.anims[/*надо как-то получить имя анимации*/].frames) // Если счётчик кадров больше чем кадров в анимации то цикл начинается заного
            {
                float x = 78 * ((frameCounter / anim_cntrl.anims[/*надо как-то получить имя анимации*/].frames) % anim_cntrl.anims[/*надо как-то получить имя анимации*/].frames);
                float y = 0;
                
                ImageCrop(&spr->image, {x, y, 78, 58});
            }
            
            ImageFlipHorizontal(&spr->image); // текстурка инвертируется по горизонтали
            trns->pos.x -= 2 * (spr->image.width / 2 - (phys->collider.x + phys->collider.width / 2));
            ++frameCounter;
        }
        else if((plr_cntrl->is_waiting) & (!plr_cntrl->is_right)) // Если флаг is_waiting == true и is_right == false, то текстурку ожидания инвертиирует по горизонтали
        {
            ImageFlipHorizontal(&spr->image);
            trns->pos.x -= padding;
        }

        UnloadTexture(spr->texture);
        spr->texture = LoadTextureFromImage(spr->image);
    }

    void Animation_control::update(float dt, Entity_id parent_id, Storage &storage)
    {
        plat::Animation *anim = storage.entities[parent_id].getComponent<Animation>();
        plat::Player_control *plr_cntrl = storage.entities[parent_id].getComponent<Player_control>();


        // прыжок идёт нах
        if(plr_cntrl->is_right) //
        {
            anim->base_image = ImageCopy(anims["RUN"].path);
        }
        else if(!plr_cntrl->is_right) //
        {
            anim->base_image = ImageCopy(anims["RUN"].path);
        }
        else if(plr_cntrl->is_waiting) // анимация по умолчанию
        {
            anim->base_image = ImageCopy(anims["STAY"].path);
        }
    }

    // Управление камерой
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
            else t->pos.y += 1;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            if (IsKeyDown(KEY_LEFT_SHIFT))
            {
                cam->scale.x *= 1 / 1.25;
                cam->scale.y *= 1 / 1.25;
            }
            else t->pos.y -= 1;
        }
    }

    void World::update(float dt, Entity_id parent_id, Storage &storage)
    {
        cur_world->Step(dt, time_settings.velocityIterations, time_settings.positionIterations);
    }
}
