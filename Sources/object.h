#pragma once

#include <random>
#include <string>

#include "constheader.h"
#include "GLSprite.h"

class Object
{
public:
    Object(int posx, int posy)
        : posx_(posx),
        posy_(posy),
        state_h_(0),
        state_w_(0),
        energy_(0),
        energy_per_sec_(0),
        stone_amount_(0),
        max_energy_(0),
        click_state_(false),
        angle_(0) 
        {
            SetSprite("circle.png");
        }
    int posx() const 
    {
        return posx_;
    }
    int posy() const
    {
        return posy_;
    }
    int state_h() const
    {
        return state_h_;
    }
    int state_w() const
    {
        return state_w_;
    }

    void SetSprite(const std::string& name, int size_w = 0, int size_h = 0);

    const std::string& sprite_name() const
    {
        return sprite_name_;
    }

    const GLSprite* sprite() const
    {
        return sprite_;
    }

    int energy_per_sec() const
    {
        return energy_per_sec_;
    }
    int energy() const
    {
        return energy_;
    }
    int stone_amount() const
    {
        return stone_amount_;
    }
    int TakeStone(int value)
    {
        if (stone_amount_ + value < 0)
            return false;
        stone_amount_ -= value;
        return value;
    }
    void ChangeEnergy(int value)
    {
        energy_ += value;
        if (energy_ < 0)
            energy_ = 0;
        if (energy_ > max_energy_)
            energy_ = max_energy_;        
    }
    int max_energy() const
    {
        return max_energy_;
    }
    bool click_state() const
    {
        return click_state_;
    }
    float angle() const
    {
        return angle_;
    }
    void Click()
    {
        click_state_ = !click_state_;
    }
    virtual void Process();
    virtual bool IsLine() const { return true; }
protected:
    bool click_state_;

    int state_h_;
    int state_w_;
    float angle_;

    int posx_;
    int posy_;
    int energy_;
    int max_energy_;
    int energy_per_sec_;
    int stone_amount_;
private:
    GLSprite* sprite_;
    std::string sprite_name_;
};

class Producer : public Object
{
public:
    Producer(int posx, int posy)
        : Object(posx, posy),
        state_counter_(0)
    {
        SetSprite("circles3.png");
        energy_per_sec_ = 1;
        max_energy_ = 100;
        state_w_ = rand() % 4;
    }
    virtual void Process() override;
private:
    int state_counter_;
};

class Keeper : public Object
{
public:
    Keeper(int posx, int posy)
        : Object(posx, posy)
    {
        SetSprite("batter.png");
        max_energy_ = 5001;
    }
    virtual void Process() override;
};

class Connector : public Object
{
public:
    Connector(int posx, int posy)
        : Object(posx, posy)
    {
        SetSprite("circles.png");
        max_energy_ = 5;
    }
};

class Stone : public Object
{
public:
    Stone(int posx, int posy)
        : Object(posx, posy)
    {
        SetSprite("stones.png");
        stone_amount_ = rand() % 9000; 
        angle_ = static_cast<float>(rand() % 360);
    }
    int TakeStone(int amount)
    {
        if (amount < 1)
            return 0;
        int value = std::min(amount, stone_amount_);
        stone_amount_ -= value;
        return value;
    }
    virtual void Process() override
    {
        Object::Process();
        state_w_ = stone_amount_ / 2900;
    }
    virtual bool IsLine() const override { return false; }
};

class Drill : public Object
{
public:
    Drill(int posx, int posy)
        : Object(posx, posy),
        state_counter_(0)
    {
        SetSprite("drill.png");
        state_w_ = rand() % 4;
        energy_per_sec_ = -2;
        max_energy_ = 50;
    }
    virtual void Process() override;
private:
    int state_counter_;
};

class Gun : public Object
{
public:
    Gun(int posx, int posy)
        : Object(posx, posy),
        state_counter_(0)
    {
        SetSprite("test.png", 4, 1);
        energy_per_sec_ = -10;
        max_energy_ = 1500;
    }
    virtual void Process() override
    {
        Object::Process();
        ++state_counter_;
        angle_ = static_cast<float>(state_counter_ % 360);
    }
private:
    int state_counter_;
};