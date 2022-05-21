#include "Bird.h"

Bird::Bird()
{
    is_falling_ = false;
    y_val_ = 1;
}

Bird::~Bird()
{
    Free();
}

void Bird::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_SPACE:
        {
            if (is_falling_ == false)
            {
                y_val_ = -5;
            }
        }
        }
    }
    else if (events.type == SDL_KEYUP)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_SPACE:
        {
            y_val_ = 5;
        }
        break;
        }
    }
}


bool Bird::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = Object::LoadImageFile(path, screen);
    return ret;
}

void Bird::Show(SDL_Renderer* des)
{
    Object::Render(des);
}

void Bird::DoFalling(SDL_Renderer* des)
{
    rect_.y += y_val_;
    if ((rect_.y + rect_.h) >= GROUND_MAP)
    {
        is_falling_ = true;
        DoGround(des);
    }
}

void Bird::DoGround(SDL_Renderer* screen)
{
    y_val_ = -5;
    rect_.y = GROUND_MAP - rect_.h;
    is_die_ = true;
}


