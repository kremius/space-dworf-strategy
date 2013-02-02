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
        speed_.x = 0;
        speed_.y = 0;
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

void Enemy::ProcessSpeed(int pixel_x_to, int pixel_y_to, int force)
{
    //auto obj = GetMap()->GetNearest(pixel_x(), pixel_y());

    int diff_x = pixel_x_to - pixel_x();
    int diff_y = pixel_y_to - pixel_y();


    float radius = sqrt(static_cast<float>(diff_x * diff_x + diff_y * diff_y));

    if (radius != 0)
    {
            speed_.x += static_cast<int>((force) * (1.0f * diff_x / radius));
            speed_.y += static_cast<int>((force) * (1.0f * diff_y / radius));
    }

    speed_.x += rand() % 3 - 1;
    speed_.y += rand() % 3 - 1;

    if (speed_.x > 0)
        speed_.x = std::min(speed_.x,  5);
    else
        speed_.x = std::max(speed_.x, -5);
    if (speed_.y > 0)
        speed_.y = std::min(speed_.y,  5);
    else
        speed_.y = std::max(speed_.y, -5);
}

void Enemy::ProcessMove()
{
    // TODO: vectors
    Move(speed_.x, speed_.y);

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

void Enemy::Process()
{
    auto obj = GetMap()->GetNearest(pixel_x(), pixel_y());
    if (obj != nullptr)
        ProcessSpeed(obj->posx() * 32, obj->posy() * 32);
    ProcessMove();
}
void Jew::Process()
{
    Enemy::Process();
    
    bool is_near = false;

    GetMap()->ForEach([&](Object* object)
    {
        if (object == nullptr)
            return;
        if (object->IsLine())
            is_near = true;
    }, pixel_x() / 32, pixel_y() / 32, 1);

    if (is_near)
        GetPlayer()->ChangeStone(-1);
}

void Rocket::Process()
{
    auto enm = GetMap()->GetEnemyHolder()->GetNearest(this->pixel_x(), this->pixel_y(), 7, 
                                                     [this](Enemy* e) 
        /*I AM KING OF SPACES*/                      {
                                                         return !e->IsRocketFriend()
                                                                && e != this;
                                                     });

    ++length_;

    if (enm != nullptr)
    {
        ProcessSpeed(enm->pixel_x(), enm->pixel_y(), 1);
        if ((abs(enm->pixel_x() - pixel_x()) + abs(enm->pixel_y() - pixel_y())) < 32)
        {
            GetMap()->GetEnemyHolder()->AddToDelete(enm);
            GetMap()->GetEnemyHolder()->AddToDelete(this);
            return;
        }
    }
    ProcessMove();

    state_w_ = (state_w_ + 1) % 4;

    if (length_ > 2 * 60)
        GetMap()->GetEnemyHolder()->AddToDelete(this);
}