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
                                  x, y,
                                  (*it)->state_w(),
                                  (*it)->state_h(),
                                  (*it)->angle());
                ++it;
            }
        }
}

void Map::EnemyHolder::Move(Enemy* enemy, int step_x, int step_y)
{

}