#include "EffectSystem.h"

#include <assert.h>

FabricProcesser* fb = nullptr;

FabricProcesser* GetFabricProcesser()
{
    assert(fb && "FabricProcesser is nullptr");
    return fb;
}

void helpers::InitFabricProcesser(FabricProcesser* fb_n)
{
    fb = fb_n;
}