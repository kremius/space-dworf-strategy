#pragma once

#include "ApproxGLImage.h"
#include "GLSprite.h"

class Screen
{
public:
    Screen(unsigned int x, unsigned int y, bool fullscreen = false);
    void Draw(const GLSprite* sprite, int x, int y, int imageW, int imageH, float angle = 0.0f);
    void Draw(const ApproxGLImage* sprite, int x_ul, int y_ul, int x_dr, int y_dr, float angle = 0.0f);
    void DrawLine(int Red1, int Green1, int Blue1, 
                  int Red2, int Green2, int Blue2, 
                  int from_x, int from_y, int to_x, int to_y);
    void Swap();
    bool Fail();
private:
    bool fail_;
    SDL_Surface* screen_;
};

Screen* GetScreen();

namespace helpers
{
    void InitScreen(Screen* screen);
}