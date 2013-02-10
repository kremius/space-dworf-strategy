#pragma once

#include <array>
#include <functional>
#include <list>
#include <set>

#include "camera.h"
#include "constheader.h"
#include "enemy.h"
#include "object.h"

class Player
{
public:
    Player() 
        : stone_amount_(60000),
          energy_amount_(0) {}
    int stone_amount() const
    {
        return stone_amount_;
    }
    void SetEnergy(int enr)
    {
        energy_amount_ = enr;
    }
    int energy_amount() const
    {
        return energy_amount_;
    }
    bool ChangeStone(int amount)
    {
        if (amount + stone_amount_ < 0)
            return false;
        stone_amount_ += amount;
        return true;
    }
private:
    int stone_amount_;
    int energy_amount_;
};

Player* GetPlayer();

namespace helpers
{
    void InitPlayer(Player* player);
}

class Map
{
public:
    typedef Object* HolderType;
    typedef std::array<HolderType, sizeHmap> InsideType;

    class EnemyHolder
    {
    public:
        typedef std::list<Enemy*> HolderType;
        typedef std::array<HolderType, sizeHmap> InsideType;
        EnemyHolder();
        bool Remove(Enemy* enemy);
        bool Add(Enemy* enemy);
        void AddToDelete(Enemy* enemy);
        void AddEnemy();
        Enemy* GetNearest(int x, int y, int radius = 7, std::function<bool(Enemy*)> predicate = [](Enemy*) {return true;});
        void Move(Enemy* enemy, int step_x, int step_y);
        void Draw();
        void Process();
        int GetTimeBeforeWave() const;
        void ProcessWave();
        void ForEach(std::function<void(Enemy*)> callback,
                 int x = -1, int y = -1, int range = -1);
        int GetEnemyAmount() const
        {
            return enemy_amount_;
        }

    private:
        int counter_;
        int enemy_amount_;
        std::set<Enemy*> delete_list_;
        std::array<InsideType, sizeWmap> holder_;
    };

    Map();
    Object* GetNearest(int x, int y);

    void AddToDelete(Object* obj)
    {
        delete_list_.insert(obj);
    }

    void Collect();

    InsideType& operator[](size_t number)
    {
        return holder_[number];
    }
    HolderType& Click(int pixel_x, int pixel_y);
    void Draw();
    void ForEach(std::function<void(Object*)> callback,
                 int posx = -1, int posy = -1, int range = -1);
    EnemyHolder* GetEnemyHolder()
    {
        return &enemy_holder_;
    }
private:
    EnemyHolder enemy_holder_;

    std::set<Object*> delete_list_;
    std::array<InsideType, sizeWmap> holder_;
};

Map* GetMap();

namespace helpers
{
    void InitMap(Map* map);
}