
#ifndef HPP_IDRAWSUBSCRIBER
#define HPP_IDRAWSUBSCRIBER

class IDrawSubscriber
{
    public:
    virtual void draw(unsigned int win_w, unsigned int win_h) = 0;
};

#endif

