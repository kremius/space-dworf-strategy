#pragma comment (lib, "WinLib/SDLmain.lib")
#pragma comment (lib, "WinLib/SDL.lib")
#pragma comment (lib, "WinLib/SDL_image.lib")
#pragma comment (lib, "WinLib/SDL_ttf.lib")
#pragma comment (lib, "WinLib/SDL_mixer.lib")

#include <math.h>

#include <iostream>
#include <random>
#include <sstream>
#include <vector>

#include "Audio.h"
#include "ASpritesClass.h"
#include "Screen.h"
#include "Text.h"

#include "camera.h"
#include "map.h"
#include "object.h"

int main(int argc, char* argv[])
{
    Screen screen(640, 480);
    helpers::InitScreen(&screen);

    ASprClass a_spr;
    helpers::InitASpr(&a_spr);

    SYSTEM_STREAM << "Begin TTF init\n";
    SYSTEM_STREAM << TTF_Init() << " - return\n";
    SYSTEM_STREAM << " End TTF init\n";
    atexit(TTF_Quit);

    Audio* au = new Audio;
    helpers::InitAudio(au);

    Camera camera;
    helpers::InitCamera(&camera);

    Map* map = new Map;
    helpers::InitMap(map);

    for (int i = 0; i != sizeWmap; ++i)
        for (int j = 0; j != sizeHmap; ++j)
            if (rand() % 10 == 1 || i == 0        || j == 0
                                 || i == sizeWmap - 1 || j == sizeHmap - 1)
                (*GetMap())[i][j] = new Stone(i, j);

    Player player;
    helpers::InitPlayer(&player);

    TextPainter texter;
    texter["Stone"].SetUpdater([&](std::string* str)
    {
        std::stringstream conv;
        conv << "Stone: " << player.stone_amount();
        str->clear();
        *str = conv.str();
    }).SetColor(100, 100, 100).SetFreq(10).SetPlace(0, 0).SetSize(16);
    texter["Energy"].SetUpdater([&](std::string* str)
    {
        std::stringstream conv;
        conv << "Energy: " << player.energy_amount();
        str->clear();
        *str = conv.str();
    }).SetColor(250, 250, 30).SetFreq(10).SetPlace(300, 0).SetSize(16);

    while(true)
    {
        bool check_new = false;
        bool click = false;
        int new_x, new_y;
        SDL_Event event;    
        while (SDL_PollEvent(&event))
        { 
            if(event.type == SDL_QUIT)
            {
                return 0;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                new_x = (event.button.x + GetCamera()->pixel_x()) / 32;
                new_y = (event.button.y + GetCamera()->pixel_y()) / 32;

                if (new_x < 0)
                    new_x = 0;
                if (new_x >= sizeWmap)
                    new_x = sizeWmap - 1;

                if (new_y < 0)
                    new_y = 0;
                if (new_y >= sizeHmap)
                    new_y = sizeHmap - 1;

                auto* obj = &GetMap()->Click(event.button.x, event.button.y);
                if (*obj == nullptr)
                    check_new = true;
                else
                    click = true;
            }
            else if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_j)
                    GetMap()->GetEnemyHolder()->AddEnemy();
            }
        }

        SDL_PumpEvents();
        auto keys = SDL_GetKeyState(NULL);
        for (int k = 0; k != 4; ++k)
        {
            if (keys[SDLK_LEFT]) 
                GetCamera()->Move(VD_LEFT);
            if (keys[SDLK_RIGHT]) 
                GetCamera()->Move(VD_RIGHT);
            if (keys[SDLK_UP]) 
                GetCamera()->Move(VD_UP);
            if (keys[SDLK_DOWN]) 
                GetCamera()->Move(VD_DOWN);
        }
        if (keys[SDLK_1] && check_new && GetPlayer()->ChangeStone(-1000))
            (*GetMap())[new_x][new_y] = new Connector(new_x, new_y);
        if (keys[SDLK_2] && check_new && GetPlayer()->ChangeStone(-5000))
            (*GetMap())[new_x][new_y] = new Producer(new_x, new_y);
        if (keys[SDLK_3] && check_new && GetPlayer()->ChangeStone(-10000))
            (*GetMap())[new_x][new_y] = new Keeper(new_x, new_y);
        if (keys[SDLK_4] && check_new && GetPlayer()->ChangeStone(-4000))
            (*GetMap())[new_x][new_y] = new Drill(new_x, new_y);
        if (keys[SDLK_6] && check_new && GetPlayer()->ChangeStone(-1000))
            (*GetMap())[new_x][new_y] = new Gun(new_x, new_y);
        if (click)
            if (keys[SDLK_5] && GetPlayer()->ChangeStone(1000))
            {
                delete (*GetMap())[new_x][new_y];
                (*GetMap())[new_x][new_y] = nullptr;
            }
            else
                (*GetMap())[new_x][new_y]->Click();
            
        glClear(GL_COLOR_BUFFER_BIT);
        
        GetMap()->Draw();
        GetMap()->ForEach([](Object* obj)
        {
            if (obj == nullptr)
                return;
            obj->Process();
        });

        GetMap()->Collect();

        GetMap()->GetEnemyHolder()->Process();
        GetPlayer()->SetEnergy(0);
        GetMap()->ForEach([](Object* obj)
        {
            if (obj == nullptr)
                return;
            GetPlayer()->SetEnergy(GetPlayer()->energy_amount() + obj->energy());
        });
        texter.Process();
        GetScreen()->Swap();
    }
    return 0;
}