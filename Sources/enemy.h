#pragma once

#include <random>
#include <string>

#include "GLSprite.h"

// TODO: double code, double code everywhere!

class Enemy
{
public:
    Enemy(int pixel_x, int pixel_y);
    int pixel_x() const
    {
        return pixel_x_;
    }
    void SetPixelX(int new_pixel_x)
    {
        pixel_x_ = new_pixel_x;
    }
    int pixel_y() const
    {
        return pixel_y_;
    }
    void SetPixelY(int new_pixel_y)
    {
        pixel_y_ = new_pixel_y;
    }
    int state_w() const
    {
        return state_w_;
    }
    int state_h() const
    {
        return state_h_;
    }
    const std::string& sprite_name() const
    {
        return sprite_name_;
    }
    void SetSprite(const std::string& name, int size_w = 0, int size_h = 0);
    GLSprite* sprite()
    {
        return sprite_;
    }
    float angle() const
    {
        return angle_;
    }
    void Push(int x, int y)
    {
        speed_.x += x;
        speed_.y += y;
    }
    int Hit(int value)
    {
        return health_ -= value;
    }
    virtual void Process();
    virtual bool IsRocketFriend() const { return false; }
    void Move(int step_x_, int step_y_);
protected:
    void ProcessHealth();
    void ProcessSpeed(int pixel_x, int pixel_y, int force = 3);
    void ProcessMove();
    virtual void OnDeath() {};
    float mass_;
    VDir speed_;
    int health_;
    std::string sprite_name_;
    GLSprite* sprite_;
    int state_h_;
    int state_w_;
    int pixel_x_;
    int pixel_y_;
    float angle_;
};

// Its just joke, srsly (or not?)
class Jew: public Enemy
{
public:
    Jew(int x, int y) : Enemy(x, y) {}
    virtual void Process() override;
};

class Rocket: public Enemy
{
public:
    Rocket(int x, int y) : Enemy(x, y) 
    {
        length_ = 0;
        SetSprite("flame1.png", 6, 1);

        pixel_x_ += rand() % 9 - 4;
        pixel_y_ += rand() % 9 - 4;
    }
    virtual void Process() override;
    virtual bool IsRocketFriend() const { return true; }
private:
    int length_;
};

class Ork: public Enemy
{
public:
    Ork(int x, int y) : Enemy(x, y)
    {
        SetSprite("ork.png");
    }
    virtual void Process() override;
};