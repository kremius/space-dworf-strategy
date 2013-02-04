#include "DeathEffect.h"

#include "ASpritesClass.h"
#include "camera.h"

RedBlood::RedBlood()
{
    sprite_ = GetASpr()->returnSpr("blood.png");
}

void RedBlood::start()
{
    counter_ = 0;
    state_w_ = 0;
}

void RedBlood::process()
{
    GetScreen()->Draw(sprite_, 
                      x_ - GetCamera()->pixel_x(), 
                      y_ - GetCamera()->pixel_y(), 
                      state_w_, 0 /*COLOR*/, angle_);

    state_w_ = (counter_ / 2) % 4;
    ++counter_;

    if (counter_ > 7)
        Release();
}

void RedBlood::end()
{
}