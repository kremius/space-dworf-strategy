#include "EffectSystem.h"

#include "GLSprite.h"

class RedBlood: public BaseEffect
{
public:
    RedBlood();
    virtual void process() override;
    virtual void start() override;
    virtual void end() override;
    RedBlood* SetPos(int x, int y, float angle)
    {
        x_ = x;
        y_ = y;
        angle_ = angle;
        return this;
    }
private:
    int x_;
    int y_;
    float angle_;
    int counter_;
    int state_w_;
    GLSprite* sprite_;
};