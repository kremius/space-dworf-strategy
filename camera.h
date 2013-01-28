#pragma once

#include "constheader.h"

class Camera
{
public:
    Camera()
        : pixel_x_(0),
        pixel_y_(0) {}
    int pixel_x() const
    {
        return pixel_x_;
    }
    int pixel_y() const
    {
        return pixel_y_;
    }
    void Move(VDir dir)
    {
        MoveTo(&pixel_x_, &pixel_y_, dir);
    }
private:
    int pixel_x_;
    int pixel_y_;
};

Camera* GetCamera();

namespace helpers
{
    void InitCamera(Camera* camera);
}