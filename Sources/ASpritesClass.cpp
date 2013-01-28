#include "ASpritesClass.h"

#include <assert.h>

GLSprite* ASprClass::returnSpr(std::string type, int size_w, int size_h)
{
    if (NODRAW)
        return nullptr;
    if(sprites.find(type) == sprites.end())
        sprites[type] = new GLSprite(type, size_w, size_h);
    return sprites[type];
}

ASprClass::ASprClass()
{
};

ASprClass* a_spr = nullptr;

ASprClass* GetASpr()
{
    assert(a_spr && "Sprite manager is nullptr");
    return a_spr;
}


void helpers::InitASpr(ASprClass* new_a_spr)
{
    a_spr = new_a_spr;
}