
#ifndef HPP_WINEWINDOW
#define HPP_WINEWINDOW

#include <list>
#include <queue>

#include <X11/Xlib.h>
#include <GL/glx.h>

#include "Indexable.hpp"
#include "Module.hpp"

class IEventSubscriber;
class IDrawSubscriber;

class WineWindow : public Module<WineWindow>
{
    friend class Module<WineWindow>;

    protected:
    WineWindow();
    virtual ~WineWindow();

    public:
    unsigned int get_width() const;
    unsigned int get_height() const;
    void enter();
    void leave();

    static unsigned int const event_subscription_max_priority;
    void add_event_subscriber(IEventSubscriber* subscriber, unsigned int priority);
    void remove_event_subscriber(IEventSubscriber* subscriber);
    
    static unsigned int const draw_subscription_max_priority;
    void add_draw_subscriber(IDrawSubscriber* drawable, unsigned int priority);
    void remove_draw_subscriber(IDrawSubscriber* drawable);

    void fake_event(XEvent const* event);
    
    // interface for wineinjection
    void on_event(XEvent& event);
    bool get_passed_event(XEvent& dest);
    bool on_draw();

    protected:
    std::list< Indexable<IEventSubscriber> > m_event_subscribers;
    std::list< Indexable<IDrawSubscriber> > m_draw_subscribers;
    
    std::queue<XEvent const*> m_passed_events;
    
    Display* m_xdpy;
    GLXContext m_xctx;
    GLXDrawable m_xdrawable;
    GLXContext m_local_xctx;
};

#endif

