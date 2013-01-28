#include "camera.h"

#include <assert.h>

Camera* camera = nullptr;

Camera* GetCamera()
{
    assert(camera && "Camera is nullptr");
    return camera;
}

void helpers::InitCamera(Camera* new_camera)
{
    camera = new_camera;
}