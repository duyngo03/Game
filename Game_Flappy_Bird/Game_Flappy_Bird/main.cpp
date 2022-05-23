#include "CommonFunction.h"
#include "Bird.h"
#include "ImpTimer.h"
#include "Pipe.h"
#include "Object.h"
#include "Text.h"


Object g_bkground;
Object g_ground;

TTF_Font* g_font_text = NULL;
TTF_Font* g_font_MENU = NULL;

bool Init()
{
    bool sucess = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Flappy Bird",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL);

    if (g_window != NULL)
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen != NULL)
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR,
                RENDER_DRAW_COLOR,
                RENDER_DRAW_COLOR,
                RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                sucess = false;
        }

        if (TTF_Init() == -1)
        {
            sucess = false;
        }

        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            sucess = false;
        }

        g_font_text = TTF_OpenFont(".\\font\\calibrib.ttf", 38);
        if (g_font_text == NULL)
        {
            return false;
        }

        g_font_MENU = TTF_OpenFont(".\\font\\calibrib.ttf", 45);
        if (g_font_MENU == NULL)
        {
            return false;
        }
    }

    return sucess;
}


bool LoadBackground()
{
    bool ret = g_bkground.LoadImageFile(".\\img\\background.png", g_screen);
    return ret;
}


void close()
{
    g_bkground.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    if (Init() == false)
    {
        return -1;
    }

    ImpTimer fps;
    bool quit = false;

    // Start Menu
    int ret_menu = SDLCommonFunction::ShowMenu(g_screen, g_font_MENU, "START", "Exit", ".\\img\\MainMenu.png", -1);
    if (ret_menu == 1) quit = true;

    if (!LoadBackground())
    {
        return -1;
    }

    bool ret = g_ground.LoadImageFile(".\\img\\base.png", g_screen);
    if (ret == false)
    {
        return -1;
    }
    g_ground.SetRect(0, GROUND_MAP);

    int count_again = 0;

again_game: //play_again

    Text text_count_;
    text_count_.setColor(255, 255, 255); 

    Bird player;
    ret = player.LoadImg(".\\img\\bird.png", g_screen);
    player.SetRect(100, 100);
    if (ret == false) return -1;

    PipeManager manage_pipe;
    ret = manage_pipe.InitPipeList(g_screen);
    if (ret == false) return -1;

    while (!quit)
    {
        fps.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                quit = true;
            }

            player.HandleInputAction(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR,
            RENDER_DRAW_COLOR,
            RENDER_DRAW_COLOR,
            RENDER_DRAW_COLOR);

        SDL_RenderClear(g_screen);

        g_bkground.Render(g_screen, NULL);

        manage_pipe.SetPlayerRect(player.GetRect());

        bool is_falling = player.GetFalling();
        if (is_falling == true)
        {
            manage_pipe.SetStopMoving(true);
        }

        manage_pipe.Render(g_screen);

        bool end_game = manage_pipe.GetColState();
        if (end_game == true)
        {
            player.SetFalling(true);
        }
        player.DoFalling(g_screen);
        player.Show(g_screen);

        Text text_count_again,again;
        again.setColor(255, 255, 255);
        std::string count_str = std::to_string(count_again);
        text_count_again.SetText(count_str);
        again.SetText("AGAIN: ");
        text_count_again.loadFromRenderedText(g_font_text, g_screen);
        text_count_again.RenderText(g_screen, 130, 10);
        again.loadFromRenderedText(g_font_text, g_screen);
        again.RenderText(g_screen, 10, 10);
        
        int score = manage_pipe.GetCount();
        std::string count_again_str = std::to_string(score);
        text_count_.SetText(count_again_str);
        text_count_.loadFromRenderedText(g_font_text, g_screen);
        text_count_.RenderText(g_screen, SCREEN_WIDTH / 2, 10);

        g_ground.Render(g_screen);
        SDL_RenderPresent(g_screen);


        // Menu END
        bool game_over = player.GetIsDie();
        if (game_over == true)
        {
            Sleep(100);
            int ret_menu = SDLCommonFunction::ShowMenu(g_screen, g_font_MENU, "Play again", "Exit", ".\\img\\gameover.png", score);

            if (ret_menu == 1)
            {
                quit = true;
                continue;
            }
            else
            {
                end_game = false;
                game_over = false;
                player.Free();
                manage_pipe.FreePipe();
                count_again ++;
                goto again_game;
            }
        }

        // FPS
        int val1 = fps.get_ticks();
        if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
        {
            SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
        }
    }

    close();
    return 0;
}
