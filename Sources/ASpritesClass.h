#ifndef ASPRITESCLASS_H
#define ASPRITESCLASS_H

#include "sprite.h"
#include <map>
#include <string>

#include "GLSprite.h"
#include "Screen.h"

class ASprClass
{
public:
    ASprClass();
    GLSprite* returnSpr(std::string type, int size_w = 0, int size_h = 0);//use it for ptr to sprite(try load sprite if isnt exist)
    TTF_Font* font;
private:
    std::map<std::string, GLSprite*> sprites;
};

ASprClass* GetASpr();

namespace helpers
{
    void InitASpr(ASprClass* a_spr);
}

#endif