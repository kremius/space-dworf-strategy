#include "enemy.h"

#include "ASpritesClass.h"
#include "map.h"

void Enemy::Move(int step_x_, int step_y_)
{
    GetMap()->GetEnemyHolder()->Move(this, step_x_, step_y_);
}
void Enemy::SetSprite(const std::string& name, int size_w, int size_h)
{
    sprite_name_ = name;
    sprite_ = GetASpr()->returnSpr(name, size_w, size_h);
}