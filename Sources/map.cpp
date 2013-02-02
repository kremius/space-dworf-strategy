#include "map.h"

#include <assert.h>

#include "ASpritesClass.h"
#include "Screen.h"

Map* map = nullptr;

Map* GetMap()
{
    assert(map && "Map is nullptr");
    return map;
}

void helpers::InitMap(Map* new_map)
{
    map = new_map;
}

Player* player = nullptr;

Player* GetPlayer()
{
    assert(player && "Map is nullptr");
    return player;
}

void helpers::InitPlayer(Player* new_player)
{
    player = new_player;
}

Map::Map()
{
    for (int i = 0; i < sizeWmap; ++i)
        for (int j = 0; j < sizeHmap; ++j)
            holder_[i][j] = nullptr;
}

Map::HolderType& Map::Click(int pixel_x, int pixel_y)
{
    int locx = (pixel_x + GetCamera()->pixel_x()) / 32;
    int locy = (pixel_y + GetCamera()->pixel_y()) / 32;

    if (locx < 0)
        locx = 0;
    if (locx >= sizeWmap)
        locx = sizeWmap - 1;

    if (locy < 0)
        locy = 0;
    if (locy >= sizeHmap)
        locy = sizeHmap - 1;

    return holder_[locx][locy];
}

void Map::Draw()
{
    for (int i = 0; i < sizeWmap; ++i)
        for (int j = 0; j < sizeHmap; ++j)
        {
            if (holder_[i][j] == nullptr || !holder_[i][j]->IsLine())
                continue;
            int x = i * 32 - GetCamera()->pixel_x();
            int y = j * 32 - GetCamera()->pixel_y();

            ForEach([&](Object* obj)
            {
                if (obj == nullptr || obj == holder_[i][j] || !obj->IsLine())
                    return;
                int to_x = obj->posx() * 32 - GetCamera()->pixel_x();
                int to_y = obj->posy() * 32 - GetCamera()->pixel_y();
                int color1 = static_cast<int>(holder_[i][j]->energy() / (holder_[i][j]->max_energy() / 250.0 + 0.01));
                int color2 = static_cast<int>(obj->energy() / (obj->max_energy() / 250.0 + 0.01));
                GetScreen()->DrawLine(250 - color1, 0, color1,
                                      250 - color2, 0, color2,
                                      x + 16, y + 16, to_x + 16, to_y + 16);                                      
            }, i, j, 3);
        }
    for (int i = 0; i < sizeWmap; ++i)
        for (int j = 0; j < sizeHmap; ++j)
        {
            if (holder_[i][j] == nullptr)
                continue;

            int x = i * 32 - GetCamera()->pixel_x();
            int y = j * 32 - GetCamera()->pixel_y();

            GetScreen()->Draw(holder_[i][j]->sprite(), 
                              x, y,
                              holder_[i][j]->state_w(),
                              holder_[i][j]->state_h(),
                              holder_[i][j]->angle());
        }
    GetEnemyHolder()->Draw();
}
 
void Map::ForEach(std::function<void(Object*)> callback,
                  int posx, int posy, int range)
{
    if (posx < 0 || posy < 0 || range < 0)
    {
        for (int i = 0; i < sizeWmap; ++i)
            for (int j = 0; j < sizeHmap; ++j)
                callback(holder_[i][j]);
        return;
    }

    for (int i = std::max(posx - range, 0); i <= std::min(posx + range, sizeWmap - 1); ++i)
        for (int j = std::max(posy - range, 0); j <= std::min(posy + range, sizeHmap - 1); ++j)
            callback(holder_[i][j]);
}

Map::EnemyHolder::EnemyHolder()
{

}

void Map::EnemyHolder::AddEnemy()
{
    for (int i = 0; i < sizeWmap; ++i)
        for (int j = 0; j < sizeHmap; ++j)
        {
            if (rand() % 3)
                continue;
            if (   i == 0   )//         || j == 0
                //|| i == sizeWmap - 1 || j == sizeHmap - 1)
                holder_[i][j].push_back(new Jew(i * 32, j * 32));
        }
}

void Map::EnemyHolder::Draw()
{
    for (int i = 0; i < sizeWmap; ++i)
        for (int j = 0; j < sizeHmap; ++j)
        {
            auto it = holder_[i][j].begin();
            while (it != holder_[i][j].end())
            {
                int x = (*it)->pixel_x() - GetCamera()->pixel_x();
                int y = (*it)->pixel_y() - GetCamera()->pixel_y();

                GetScreen()->Draw((*it)->sprite(), 
                                  x - 16, y - 16,
                                  (*it)->state_w(),
                                  (*it)->state_h(),
                                  (*it)->angle());
                ++it;
            }
        }
}

void Map::EnemyHolder::Process()
{
    for (int i = 0; i < sizeWmap; ++i)
        for (int j = 0; j < sizeHmap; ++j)
        {
            auto it = holder_[i][j].begin();
            while (it != holder_[i][j].end())
            {
                (*(it++))->Process();
            }
        }

     for (auto it = delete_list_.begin(); it != delete_list_.end(); ++it)
     {
         Remove(*it);
         delete *it;
     }

     delete_list_.erase(delete_list_.begin(), delete_list_.end());
};

void Map::EnemyHolder::Move(Enemy* enemy, int step_x, int step_y)
{
    int old_x = enemy->pixel_x();
    int old_y = enemy->pixel_y();

    int new_x = old_x + step_x;
    int new_y = old_y + step_y;

    int old_posx = old_x / 32;
    int old_posy = old_y / 32;

    int new_posx = new_x / 32;
    int new_posy = new_y / 32;

    if (old_posx >= sizeWmap || old_posx < 0)
        return; // WTF
    if (old_posy >= sizeHmap || old_posy < 0)
        return;

    if (new_posx >= sizeWmap || new_posx < 0)
        return;
    if (new_posy >= sizeHmap || new_posy < 0)
        return;

    if (new_x < 0 || new_y < 0)
        return;

    enemy->SetPixelX(new_x);
    enemy->SetPixelY(new_y);

    if (new_posx == old_posx && new_posy == old_posy)
        return;

    // Remove old position
    auto& old_cont = holder_[old_posx][old_posy];
    auto it = std::find(old_cont.begin(), old_cont.end(), enemy);
    assert(it != old_cont.end() && "Cannot find old position of enemy");
    old_cont.erase(it);
    
    auto& new_cont = holder_[new_posx][new_posy];
    new_cont.push_back(enemy);
}

bool Map::EnemyHolder::Remove(Enemy* enemy)
{
    int rem_x = enemy->pixel_x() / 32;
    int rem_y = enemy->pixel_y() / 32;

    auto& cont = holder_[rem_x][rem_y];
    auto it = std::find(cont.begin(), cont.end(), enemy);
    assert(it != cont.end() && "Cannot find position of enemy");
    cont.erase(it);
    return true;
}

bool Map::EnemyHolder::Add(Enemy* enemy)
{
    int add_x = enemy->pixel_x() / 32;
    int add_y = enemy->pixel_y() / 32;

    if (   add_x < 0         || add_y < 0
        || add_x >= sizeWmap || add_y >= sizeHmap) // TODO: COPY PASTE COPY PASTE STOP IT
        return false;
    holder_[add_x][add_y]
        .push_back(enemy);
    return true;
}

void Map::EnemyHolder::AddToDelete(Enemy* enemy)
{
    delete_list_.insert(enemy);
}

Enemy* Map::EnemyHolder::GetNearest(int x, int y, int radius, std::function<bool(Enemy*)> predicate)
{
    int from_posx = std::max(x / 32 - radius, 0);
    int from_posy = std::max(y / 32 - radius, 0);

    int to_posx = std::min(x / 32 + radius, sizeWmap - 1);
    int to_posy = std::min(y / 32 + radius, sizeHmap - 1);

    int min_radius = 999999999;
    Enemy* retval = nullptr;

    for (int i = from_posx; i <= to_posx; ++i)
        for (int j = from_posy; j <= to_posy; ++j)
        {
            auto it = holder_[i][j].begin();
            while (it != holder_[i][j].end())
            {
                if (predicate(*it))
                {
                    int new_radius = (*it)->pixel_x() * (*it)->pixel_x()
                                    + (*it)->pixel_y() * (*it)->pixel_y();
                    if (new_radius < min_radius)
                    {
                        min_radius = new_radius;
                        retval = *it;
                    }
                }
                ++it;
            }
        }
    return retval;
}

Object* Map::GetNearest(int x, int y)
{
    //int posx = x / 32;
    //int posy = y / 32;

    float min_radius = 999999999.0f;
    Object* nearest_object = nullptr;

    GetMap()->ForEach([&](Object* object)
    {
        // TODO: new algorithm (from center x, y to border)
        if (object == nullptr || !object->IsLine())
            return;
        int diff_x = object->posx() * 32 - x;
        int diff_y = object->posy() * 32 - y;
        float new_min_radius = static_cast<float>(diff_x * diff_x + diff_y * diff_y);
        if (new_min_radius < min_radius)
        {
            min_radius = new_min_radius;
            nearest_object = object;
        }
    });

    return nearest_object;
}