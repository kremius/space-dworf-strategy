#include "enemy.h"

#include "ASpritesClass.h"
#include "map.h"

Enemy::Enemy(int pixel_x, int pixel_y)
    : pixel_x_(pixel_x),
    pixel_y_(pixel_y),
    angle_(0.0f),
    state_h_(0),
    state_w_(0) 
    {
        SetSprite("jew.png");
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