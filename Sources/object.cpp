#include "object.h"

#include "ASpritesClass.h"
#include "map.h"

void Object::SetSprite(const std::string& name, int size_w, int size_h)
{
    sprite_name_ = name;
    sprite_ = GetASpr()->returnSpr(name, size_w, size_h);
}

void Object::Process()
{
    energy_ += energy_per_sec_;
    if (energy_ < 0)
        energy_ = 0;
    if (energy_ > max_energy_)
        energy_ = max_energy_;

    GetMap()->ForEach([this](Object* obj)
    {
        if (obj == nullptr || obj == this)
            return;
        if (obj->energy() < energy())
        {
            int change_value = std::min(obj->max_energy() - obj->energy(), std::min(1, energy()));
            obj->ChangeEnergy(change_value);
            ChangeEnergy(change_value * -1);
        }
    }, posx(), posy(), 3);

    if (health_ < 0)
    {
        GetMap()->AddToDelete(this);
    }
};

void Producer::Process()
{
    Object::Process();
    GetMap()->ForEach([this](Object* obj)
    {
        if (obj == nullptr || obj == this)
            return;
        if (obj->energy() < obj->max_energy() && obj->energy_per_sec() <= 0)
        {
            int change_value = std::min(obj->max_energy() - obj->energy(), std::min(3, energy()));
            obj->ChangeEnergy(change_value);
            ChangeEnergy(change_value * -1);
        }
    }, posx(), posy(), 3);
    if (max_energy() != energy())
        ++state_counter_;
    angle_ = static_cast<float>(state_counter_ % 360);
}

void Keeper::Process()
{
    Object::Process();
    state_w_ = energy_ / ((max_energy_ - 21) / 3);
    if (click_state() == true)
    {
        GetMap()->ForEach([this](Object* obj)
        {
            if (obj == nullptr || obj == this)
                return;
            if (obj->energy() <= 0 || energy() == max_energy())
                return;
            int change_value = std::min(obj->energy(), 3);
            obj->ChangeEnergy(change_value * -1);
            ChangeEnergy(change_value);
        }, posx(), posy(), 3);
    }
}

void Drill::Process()
{
    Object::Process();
    angle_ = static_cast<float>(state_counter_ % 360);

    if (click_state() == true)
    {
        GetMap()->ForEach([this](Object* obj)
        {
            if (obj == nullptr || obj == this)
                return;
            if (obj->energy() <= 0 || energy() == max_energy())
                return;
            int change_value = std::min(obj->energy(), 3);
            obj->ChangeEnergy(change_value * -1);
            ChangeEnergy(change_value);
        }, posx(), posy(), 3);
    }
    if (energy() == 0)
        return;
    bool is_work = false;
    GetMap()->ForEach([this, &is_work](Object* obj)
    {
        if (obj == nullptr || obj == this)
            return;
        if (obj->stone_amount() <= 0)
            return;
        GetPlayer()->ChangeStone(obj->TakeStone(1));
        is_work = true;
    }, posx(), posy(), 1);
    if (is_work)
    {
       energy_per_sec_ = -2;
       state_counter_ += 4;
    }
    else
    {
        energy_per_sec_ = 0;
    }
}

void Gun::Process()
{
    Object::Process();

    auto enm = GetMap()->GetEnemyHolder()->GetNearest(
                         this->posx() * 32 + 16, this->posy() * 32 + 16, 20, 
                         [](Enemy* e) { return !e->IsRocketFriend();});

    if (enm != nullptr)
    {
        int diff_x = enm->pixel_x() - posx() * 32;
        int diff_y = enm->pixel_y() - posy() * 32;

        float angle_rad = (angle_ - 90.0) * (3.14f / 180.0f);

        int x = 100 * cos(angle_rad);
        int y = 100 * sin(angle_rad);

        if ((diff_x * y - diff_y * x) > 0)
            angle_ -= 4.0 + rand() % 32;
        else
            angle_ += 4.0 + rand() % 32;
    }
    if (enm != nullptr)
    {
        for (int i = 0; i < 5; ++i)
        {
            auto new_item = new Rocket(posx() * 32 + 16, posy() * 32 + 16);
            new_item->Push(5 * cos((angle_ - 90.0) * (3.14f / 180.0f)), 5 * sin((angle_ - 90.0) * (3.14f / 180.0f)));
            GetMap()->GetEnemyHolder()->Add(new_item);
        }
    }
}