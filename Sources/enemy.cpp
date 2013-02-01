#include "enemy.h"

#include <math.h>

#include "ASpritesClass.h"
#include "map.h"

Enemy::Enemy(int pixel_x, int pixel_y)
    : pixel_x_(pixel_x),
    pixel_y_(pixel_y),
    angle_(0.0f),
    state_h_(0),
    state_w_(0),
    mass_(0.1f)
    {
        speed_.x = rand() % 3 - 1;
        speed_.y = rand() % 3 - 1;
        speed_.z = 0;
        SetSprite("jew.png", 1, 1);
    }

void Enemy::Move(int step_x_, int step_y_)
{
    GetMap()->GetEnemyHolder()->Move(this, step_x_, step_y_);
}
void Enemy::SetSprite(const std::string& name, int size_w, int size_h)
{
    sprite_name_ = name;
    sprite_ = GetASpr()->returnSpr(name, size_w, size_h);
}

void Enemy::ProcessSpeed()
{
    auto obj = GetMap()->GetEnemyHolder()->GetNearest(pixel_x(), pixel_y());

    int diff_x = 0;
    int diff_y = 0;
    if (obj != nullptr)
    {
        diff_x = obj->posx() * 32 - pixel_x();
        diff_y = obj->posy() * 32 - pixel_y();
    }


    float radius = sqrt(static_cast<float>(diff_x * diff_x + diff_y * diff_y));

    if (radius != 0)
    {
            speed_.x += static_cast<int>(3) * (1.0f * diff_x / radius);
            speed_.y += static_cast<int>(3) * (1.0f * diff_y / radius);
    }

    speed_.x += rand() % 15 - 7;
    speed_.y += rand() % 15 - 7;

    if (speed_.x > 0)
        speed_.x = std::min(speed_.x,  5);
    else
        speed_.x = std::max(speed_.x, -5);
    if (speed_.y > 0)
        speed_.y = std::min(speed_.y,  5);
    else
        speed_.y = std::max(speed_.y, -5);
}

void Enemy::Process()
{
    // TODO: vectors
    
    Move(speed_.x, speed_.y);

    ProcessSpeed();

    if (speed_.x == 0 && speed_.y == 0)
        ;
    else
    {
        float help_angle = atan(speed_.y / (speed_.x * 1.0f)) * (180.0f / 3.14f);
        float sum_angle = 0.0f;
        if (speed_.x < 0)
            sum_angle = 180.0f;
        angle_ = sum_angle + help_angle + 90.0f;
    }
}