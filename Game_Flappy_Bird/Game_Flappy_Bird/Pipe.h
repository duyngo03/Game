// Lớp tạo các cột
#ifndef PIPE_H_
#define PIPE_H_

#include "CommonFunction.h"
#include "Object.h"


class Pipe : Object
{
public:
    Pipe();
    ~Pipe();
    bool LoadImg(std::string path, SDL_Renderer* screen);
    void SetPos(const int& xp, const int& yp);
    void SetXPos(const int& xp);
    SDL_Rect GetRectPos() const { return this->GetRect(); }
    void ShowImg(SDL_Renderer* screen);
    void DoRun(UINT& x_val);
    bool GetStateBack() const { return is_back_; }
    void SetBack(bool b) { is_back_ = b; }
private:
    bool is_back_;
    int x_pos_;
    int y_pos_;
};



class PipeInit
{
public:
    PipeInit();
    ~PipeInit();
    bool Init(SDL_Renderer* screen, const int& xp);
    void RenderImg(SDL_Renderer* screen);
    void DoMoving();
    bool GetIsBack() const { return is_back_; }
    void SetIsBack(bool isBack);
    SDL_Rect GetTopRect();
    void SetRectVal(const unsigned int& xp);
    void GetRectSlot();
    bool CheckPass(SDL_Rect rect);
    bool GetIsPass() const { return is_pass_; }
    void SetIsPass(const bool& pp) { is_pass_ = pp; }
    bool CheckCol(SDL_Rect pl_rect);
private:
    Pipe Top_;
    Pipe Bot_;
    unsigned int x_val_;
    bool is_back_;
    SDL_Rect slot_rect_;
    bool is_pass_;
};


class PipeManager
{
public:
    PipeManager();
    ~PipeManager();
    bool InitPipeList(SDL_Renderer* screen);
    void Render(SDL_Renderer* screen);
    void SetStopMoving(const bool& stop);
    void SetPlayerRect(SDL_Rect pRect) { player_rect_ = pRect; }
    bool GetColState() const { return is_col_; }
    int GetCount() const { return m_count; }
    void FreePipe();
private:
    std::vector<PipeInit*> PipeList;
    unsigned int xp_max_;
    bool stop_moving_;
    SDL_Rect player_rect_;
    int m_count;
    bool is_col_;
};
#endif

