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
        health_(10),
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

    void Hit(int value)
    {
        health_ -= value;
    }

    virtual void Process();

    // TODO: WTF? Rename 
    virtual bool IsLine() const { return true; }
protected:
    int health_;

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

class Enemy;

class Gun : public Object
{
public:
    Gun(int posx, int posy)
        : Object(posx, posy),
        state_counter_(0)
    {
        SetSprite("platform.png", 1, 1);
        energy_per_sec_ = -10;
        max_energy_ = 100;
    }
    virtual void Process() override 
    {
        Object::Process();
    };
protected:
    template<class T>
    void ProcessFire(int radius)
    {
        if (click_state() == true)
        {
            GetMap()->ForEach([this](Object* obj)
            {
                if (obj == nullptr || obj == this)
                    return;
                if (obj->energy() <= 0 || energy() > max_energy() - 10)
                    return;
                int change_value = std::min(obj->energy(), 3);
                obj->ChangeEnergy(change_value * -1);
                ChangeEnergy(change_value - 1);
            }, posx(), posy(), 3);
        }

        auto enm = GetMap()->GetEnemyHolder()->GetNearest(
                             this->posx() * 32 + 16, this->posy() * 32 + 16, radius, 
                             [](Enemy* e) { return !e->IsRocketFriend();});

        if (energy() <= 10 || enm == nullptr)
        {
            energy_per_sec_ = 0;
            return;
        }

        energy_per_sec_ = -10;
       int diff_x = enm->pixel_x() - posx() * 32;
        int diff_y = enm->pixel_y() - posy() * 32;

        float angle_rad = (angle_ - 90.0f) * (3.14f / 180.0f);

        int x = static_cast<int>(100 * cos(angle_rad));
        int y = static_cast<int>(100 * sin(angle_rad));

        if ((diff_x * y - diff_y * x) > 0)
            angle_ -= 7.0f + rand() % 32;
        else
            angle_ += 7.0f + rand() % 32;

        for (int i = 0; i < 3; ++i)
        {
            auto e = new T(posx() * 32 + 16, posy() * 32 + 16);
            e->Push(static_cast<int>(5 * cos((angle_ - 90.0f) * (3.14f / 180.0f))),
                            static_cast<int>(5 * sin((angle_ - 90.0f) * (3.14f / 180.0f))));
            GetMap()->GetEnemyHolder()->Add(e);
        }
    }
private:
    int state_counter_;
};

class Firethrower : public Gun
{
public:
    Firethrower(int posx, int posy)
        : Gun(posx, posy) {}
    virtual void Process() override;
};

/*class Rocketgun : public Gun
{
public:
    Rocketgun(int posx, int posy)
        : Gun(posx, posy) {}
    virtual void Process() override;
};*/