
#ifndef HPP_IEVENTSUBSCRIBER
#define HPP_IEVENTSUBSCRIBER

#include <X11/Xlib.h>

class IEventSubscriber
{
    public:
    virtual bool inject_event(XEvent& event) = 0;
};

#endif

