#include "Pipe.h"

int g_pos[] = { 450, 750, 1050 };


Pipe::Pipe()
{
    is_back_ = false;
}

Pipe::~Pipe()
{

}

bool Pipe::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = Object::LoadImageFile(path, screen);
    return ret;
}

void Pipe::SetPos(const int& xp, const int& yp)
{
    this->SetRect(xp, yp);
}

void Pipe::SetXPos(const int& xp)
{
    SDL_Rect rect = this->GetRect();
    this->SetRect(xp, rect.y);
}

void Pipe::ShowImg(SDL_Renderer* screen)
{
    this->Render(screen);
}

void Pipe::DoRun(UINT& x_val)
{
    this->rect_.x += x_val;
    if (rect_.x + rect_.w < 0)
    {
        is_back_ = true;
    }
}

PipeInit::PipeInit()
{
    x_val_ = -3;
    is_back_ = false;
    is_pass_ = false;
}

PipeInit::~PipeInit()
{

}

bool PipeInit::Init(SDL_Renderer* screen, const int& xp)
{
    int number = SDLCommonFunction::MyRandom(1, 10);
    int number2 = number;
    number = number * 14;

    bool top = Top_.LoadImg("img//top.png", screen);
    bool bot = Bot_.LoadImg("img//bottom.png", screen);

    if (number2 % 2 == true)
    {
        Top_.SetPos(xp, -300 - number);
        Bot_.SetPos(xp, GROUND_MAP - 220 - number);
    }
    else
    {
        Top_.SetPos(xp, -300 + number);
        Bot_.SetPos(xp, GROUND_MAP - 220 + number);
    }

    if (top && bot)
        return true;
    return false;
}

void PipeInit::RenderImg(SDL_Renderer* screen)
{
    Top_.ShowImg(screen);
    Bot_.ShowImg(screen);
}

void PipeInit::DoMoving()
{
    Top_.DoRun(x_val_);
    Bot_.DoRun(x_val_);
    if (Top_.GetStateBack() || Bot_.GetStateBack())
    {
        is_back_ = true;
    }
}

SDL_Rect PipeInit::GetTopRect()
{
    return Top_.GetRectPos();
}

void PipeInit::SetRectVal(const UINT& xp)
{
    Top_.SetXPos(xp);
    Bot_.SetXPos(xp);
}

void PipeInit::SetIsBack(bool isBack)
{
    is_back_ = isBack;
    Top_.SetBack(isBack);
    Bot_.SetBack(isBack);
}

void PipeInit::GetRectSlot()
{
    SDL_Rect rect_top = Top_.GetRectPos();

    slot_rect_.x = rect_top.x + rect_top.w;
    slot_rect_.y = rect_top.y + rect_top.h;
    slot_rect_.w = 5;
    slot_rect_.h = 160;
}

bool PipeInit::CheckPass(SDL_Rect rect)
{
    bool ret = false;
    ret = SDLCommonFunction::CheckCollision(rect, slot_rect_);
    return ret;
}

bool PipeInit::CheckCol(SDL_Rect pl_rect)
{
    bool ret1 = SDLCommonFunction::CheckCollision(pl_rect, Top_.GetRectPos());
    bool ret2 = SDLCommonFunction::CheckCollision(pl_rect, Bot_.GetRectPos());

    if (ret1 || ret2)
    {
        return true;
    }

    return false;
}

PipeManager::PipeManager()
{
    xp_max_ = 0;
    stop_moving_ = false;
    m_count = 0;
    is_col_ = false;
}

PipeManager::~PipeManager()
{
    FreePipe();
}

void PipeManager::FreePipe()
{
    for (int i = 0; i < PipeList.size(); i++)
    {
        PipeInit* Pipe_ = PipeList.at(i);
        delete Pipe_;
        Pipe_ = NULL;
    }

    PipeList.clear();
}

bool PipeManager::InitPipeList(SDL_Renderer* screen)
{
    PipeInit* Pipe1 = new PipeInit();
    PipeInit* Pipe2 = new PipeInit();
    PipeInit* Pipe3 = new PipeInit();

    int ret = Pipe1->Init(screen, g_pos[0]);
    if (ret == false) return false;
    ret = Pipe2->Init(screen, g_pos[1]);
    if (ret == false) return false;
    ret = Pipe3->Init(screen, g_pos[2]);
    if (ret == false) return false;

    PipeList.push_back(Pipe1);
    PipeList.push_back(Pipe2);
    PipeList.push_back(Pipe3);

    xp_max_ = 2;
    return true;
}

void PipeManager::SetStopMoving(const bool& stop)
{
    stop_moving_ = stop;
}

void PipeManager::Render(SDL_Renderer* screen)
{
    for (int i = 0; i < PipeList.size(); i++)
    {
        PipeInit* Pipe_ = PipeList.at(i);

        Pipe_->GetRectSlot();

        if (!stop_moving_)
        {
            Pipe_->DoMoving();

            bool ret = Pipe_->GetIsBack();
            if (ret == true)
            {
                PipeInit* endPipe = PipeList.at(xp_max_);
                SDL_Rect end_rect = endPipe->GetTopRect();
                unsigned int xp = end_rect.x + 300;
                Pipe_->SetRectVal(xp);
                Pipe_->SetIsBack(false);
                Pipe_->SetIsPass(false);
                xp_max_ = i;
            }

            bool isCol = Pipe_->CheckCol(player_rect_);
            if (isCol == true)
            {
                is_col_ = true;
                Mix_Chunk* beep_sound = Mix_LoadWAV("sound//punch.wav");
                if (beep_sound != NULL)
                    Mix_PlayChannel(-1, beep_sound, 0);
                break;
            }

            ret = Pipe_->CheckPass(player_rect_);
            if (ret == true)
            {
                if (Pipe_->GetIsPass() == false)
                {
                    Mix_Chunk* beep_sound = Mix_LoadWAV("sound//ting.wav");
                    if (beep_sound != NULL)
                        Mix_PlayChannel(-1, beep_sound, 0);
                    m_count++;
                    Pipe_->SetIsPass(true);
                }
            }
        }

        Pipe_->RenderImg(screen);

    }
}

