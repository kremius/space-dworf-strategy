#pragma once

#include <random>
#include <string>

#include "GLSprite.h"

class Enemy
{
public:
    Enemy(int pixel_x, int pixel_y)
        : pixel_x_(pixel_x),
        pixel_y_(pixel_y),
        angle_(0.0f),
        state_h_(0),
        state_w_(0) 
        {
            SetSprite("ork.png");
        }
    int pixel_x() const
    {
        return pixel_x_;
    }
    int pixel_y() const
    {
        return pixel_y_;
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
    virtual void Process()
    {
        Move(rand() % 3 - 1, rand() % 3 - 1);
    }
    void Move(int step_x_, int step_y_);
private:
    int health_;
    std::string sprite_name_;
    GLSprite* sprite_;
    int state_h_;
    int state_w_;
    int pixel_x_;
    int pixel_y_;
    float angle_;
};