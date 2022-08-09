#include "animation.h"

namespace plat
{
    Animation::Animation(const std::string& path)
    {
        Image base_image = LoadImage(path);
    }

    Animation_control::Animation_control()
    {}

//-----------------------------------------------------------------------------------------------
    
    std::string Animation::get_component_type()
    {
        return std::type_index(typeid(Animation)).name();
    }

    std::string Animation_control::get_component_type()
    {
        return std::type_index(typeid(Animation_control)).name();
    }

//-----------------------------------------------------------------------------------------------
    
    void Animation::update(float dt, Entity_id parent_id, Storage &storage)
    {
        plat::Sprite *spr = storage.entities[parent_id].getComponent<Sprite>();
        plat::Animation_control *anim_cntrl = storage.entities[parent_id].getComponent<Animation_control>();
        plat::Physics *phys = storage.entities[parent_id].getComponent<Physics>();
        plat::Transform *trns = storage.entities[parent_id].getComponent<Transform>();
        plat::Player_control *plr_cntrl = storage.entities[parent_id].getComponent<Player_control>();

        const int padding = 2 * (this->base_image.width / 2 - (phys->collider.x + phys->collider.width / 2));

        spr->image = ImageCopy(this->base_image);

        if(plr_cntrl->is_flying)
        {
            if(!plr_cntrl->is_right)
            {
                ImageFlipHorizontal(&spr->image);
                trns->pos.x -= padding;
            }
        }
        else if(plr_cntrl->is_right)
        {
            if (/*frameCounter >= frameDelay*/)
            {
                float x = 78 * (/*(frameCounter / animFrames) % animFrames*/);
                float y = 0;

                ImageCrop(&spr->image, {x, y, 78, 58});
            }

            //++frameCounter;
        }
        else if(!plr_cntrl->is_right)
        {
            if (/*frameCounter >= frameDelay*/)
            {
                float x = 78 * (/*(frameCounter / animFrames) % animFrames*/);
                float y = 0;
                
                ImageCrop(&spr->image, {x, y, 78, 58});
            }
            
            ImageFlipHorizontal(&spr->image);
            trns->pos.x -= 2 * (spr->image.width / 2 - (phys->collider.x + phys->collider.width / 2));
            //++frameCounter;
        }
        else if(plr_cntrl->is_waiting)
        {
            if(!plr_cntrl->is_right)
            {
                ImageFlipHorizontal(&spr->image);
                trns->pos.x -= padding;
            }
        }

        UnloadTexture(spr->texture);
        spr->texture = LoadTextureFromImage(spr->image);
    }

    void Animation_control::update(float dt, Entity_id parent_id, Storage &storage)
    {
        plat::Animation *anim = storage.entities[parent_id].getComponent<Animation>();
        plat::Player_control *plr_cntrl = storage.entities[parent_id].getComponent<Player_control>();

        if(plr_cntrl->is_flying)
        {
            anim->base_image = ImageCopy();
        }
        else if(plr_cntrl->is_right)
        {
            anim->base_image = ImageCopy(run);
        }
        else if(!plr_cntrl->is_right)
        {
            anim->base_image = ImageCopy(run);
        }
        else if(plr_cntrl->is_waiting)
        {
            anim->base_image = ImageCopy(stay);
        }
    }
//-----------------------------------------------------------------------------------------------
}