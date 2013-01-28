#include "sprite.h"

//#include <SDL_opengl.h>
//#include <gl\

void SetMasks(Uint32* rmask, Uint32* gmask, Uint32* bmask, Uint32* amask)
{
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
      *rmask = 0xff000000;
      *gmask = 0x00ff0000;
      *bmask = 0x0000ff00;
      *amask = 0x000000ff;
    #else
      *rmask = 0x000000ff;
      *gmask = 0x0000ff00;
      *bmask = 0x00ff0000;
      *amask = 0xff000000;
    #endif
}

bool CSprite::init(InitSprite data)
{
    SDL_Surface* temp = IMG_Load(data.imgFile.c_str());
    if(temp == NULL) return 0;
    numFrameH = data.numFrameH;
    numFrameW = data.numFrameW;
    if((numFrameH == 0) || (numFrameW == 0))
    {
        w = TITLE_SIZE;
        h = TITLE_SIZE;
        numFrameW = temp->w / TITLE_SIZE;
        numFrameH = temp->h / TITLE_SIZE;
        SYSTEM_STREAM << numFrameW << " x " << numFrameH << " - loaded " << data.imgFile.c_str() << std::endl;
    }
    else
    {
        w = temp->w / numFrameW;
        h = temp->h / numFrameH;
    }
    Uint32 rmask, gmask, bmask, amask;
    SetMasks(&rmask, &gmask, &bmask, &amask);

    SDL_Rect place;
    place.x = 0;
    place.y = 0;
    place.h = h;
    place.w = w;
    frames = new SDL_Surface*[numFrameH * numFrameW];
    if (temp->flags & SDL_SRCALPHA)
        temp->flags = temp->flags ^ SDL_SRCALPHA;//for alpha canal
    SDL_Surface* local;
    for(int j = 0; j < numFrameH; ++j)
    {
        place.y = j * h;
        for(int i = 0; i < numFrameW; ++i)
        {
            place.x = i * w;
            local = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, bmask, gmask, rmask, amask);
            SDL_BlitSurface(temp, &place, local, NULL);
            frames[i * numFrameH + j] = local;
        }
    }
    SDL_FreeSurface(temp);
    return 1;
}

CSprite::CSprite()
{
    h = 0;
    w = 0;
    numFrameH = 0;
    numFrameW = 0;
    frames = nullptr;
}

CSprite::~CSprite()
{
    if (frames)
    {
        for (int j = 0; j < numFrameH; j++)
            for (int i = 0; i < numFrameW; i++)
                SDL_FreeSurface(frames[i * numFrameH + j]);

        delete[] frames;
    }
}
