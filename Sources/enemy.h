#pragma once

#include <random>
#include <string>



class Enemy
{
public:
    Enemy(int pixel_x, int pixel_y)
        : pixel_x_(pixel_x),
        pixel_y_(pixel_y),
        angle_(0.0f),
        sprite_("ork.png") {}
    int pixel_x() const
    {
        return pixel_x_;
    }
    int pixel_y() const
    {
        return pixel_y_;
    }
    const std::string& sprite() const
    {
        return sprite_;
    }
    float angle() const
    {
        return angle_;
    }
    virtual void Process()
    {
        pixel_x_ += (rand() % 3 - 1);
        pixel_y_ += (rand() % 3 - 1);
    }
private:
    int health_;
    std::string sprite_;
    int pixel_x_;
    int pixel_y_;
    float angle_;
};