#include "DeathEffect.h"

#include <math.h>

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
                      x_ - GetCamera()->pixel_x() - 16, 
                      y_ - GetCamera()->pixel_y() - 16, 
                      state_w_, 0 /*COLOR*/, angle_);

    state_w_ = (counter_ / 2) % 4;
    ++counter_;

    if (counter_ > 7)
        Release();
}

void RedBlood::end()
{
}

Explosion::Explosion()
{
    sprite_ = GetASpr()->returnSpr("explosion.png");
}

void Explosion::start()
{
    counter_ = 0;
    state_w_ = 0;
}

void Explosion::process()
{
    for (float add_angle = 0.0f; add_angle <= 360.0f; add_angle += 60.0f)
        GetScreen()->Draw(sprite_, 
                          x_ - GetCamera()->pixel_x() - 16 + counter_ * cos((add_angle + angle_) * (3.14f / 180.0f)), 
                          y_ - GetCamera()->pixel_y() - 16 + counter_ * sin((add_angle + angle_) * (3.14f / 180.0f)), 
                          state_w_, 0 /*COLOR*/, angle_ + add_angle);

    state_w_ = (counter_ / 4) % 7;
    ++counter_;

    if (counter_ > 27)
        Release();
}

void Explosion::end()
{
}