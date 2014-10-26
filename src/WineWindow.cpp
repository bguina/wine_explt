
#include <algorithm>
#include <cstring>

#include "Indexable.hpp"
#include "IEventSubscriber.hpp"
#include "IDrawSubscriber.hpp"
#include "WineWindow.hpp"

SINGLETON_DECLARE(WineWindow);

template class Indexable<IEventSubscriber>;
template class Indexable<IDrawSubscriber>;

unsigned int const WineWindow::event_subscription_max_priority = 100;//Indexable<IEventSubscriber>::max_priority;
unsigned int const WineWindow::draw_subscription_max_priority = 100;//Indexable<IDrawSubscriber>::max_priority;

WineWindow::WineWindow()
    : m_xdpy(glXGetCurrentDisplay()), m_xctx(glXGetCurrentContext()), m_xdrawable(glXGetCurrentDrawable()), m_local_xctx(0)
{
    int screen = -1;
    int attribs[] = {GLX_FBCONFIG_ID, -1, None};
    int dummy;
    GLXFBConfig* fb;
    XVisualInfo* vis; 

    glXQueryContext(m_xdpy, m_xctx, GLX_SCREEN, &screen);
    glXQueryContext(m_xdpy, m_xctx, GLX_FBCONFIG_ID, &attribs[1]);
    
    assert(fb = glXChooseFBConfig(m_xdpy, screen, attribs, &dummy));
    assert(vis = glXGetVisualFromFBConfig(m_xdpy, *fb));
    assert(m_local_xctx = glXCreateContext(m_xdpy, vis, 0, True));

    enter();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    leave();
}

WineWindow::~WineWindow()
{
    //TODO
}

unsigned int WineWindow::get_width() const
{
    unsigned int result;

    glXQueryDrawable(m_xdpy, m_xdrawable, GLX_WIDTH, &result);
    return result;
}

unsigned int WineWindow::get_height() const
{
    unsigned int result;

    glXQueryDrawable(m_xdpy, m_xdrawable, GLX_HEIGHT, &result);
    return result;
}

void WineWindow::enter()
{
    assert(glXMakeCurrent(m_xdpy, m_xdrawable, m_local_xctx));
}

void WineWindow::leave()
{
    assert(glXMakeCurrent(m_xdpy, m_xdrawable, m_xctx));
}

void WineWindow::add_event_subscriber(IEventSubscriber* subscriber, unsigned int priority)
{
    Indexable<IEventSubscriber> handler(subscriber, priority);

    if (std::find(m_event_subscribers.begin(), m_event_subscribers.end(), handler) == m_event_subscribers.end())
    {
        std::list< Indexable<IEventSubscriber> > new_element;

        new_element.push_front(handler);
        m_event_subscribers.merge(new_element);
    }
}

void WineWindow::remove_event_subscriber(IEventSubscriber* subscriber)
{
    m_event_subscribers.remove(Indexable<IEventSubscriber>(subscriber, 0));
}

void WineWindow::add_draw_subscriber(IDrawSubscriber* draw_subscriber, unsigned int priority)
{
    Indexable<IDrawSubscriber> handler(draw_subscriber, priority);

    if (std::find(m_draw_subscribers.begin(), m_draw_subscribers.end(), handler) == m_draw_subscribers.end())
    {
        std::list< Indexable<IDrawSubscriber> > new_element;

        new_element.push_front(handler);
        m_draw_subscribers.merge(new_element);
    }
}

void WineWindow::remove_draw_subscriber(IDrawSubscriber* draw_subscriber)
{
    m_draw_subscribers.remove(Indexable<IDrawSubscriber>(draw_subscriber, 0));
}

void WineWindow::fake_event(XEvent const* event)
{
    m_passed_events.push(event);
}

void WineWindow::on_event(XEvent& event)
{
    std::list< Indexable<IEventSubscriber> >::iterator it;

    for (it = m_event_subscribers.begin(); it != m_event_subscribers.end(); it++)
        if (it->indexed().inject_event(event))
            break;
    
    if (it == m_event_subscribers.end())
        this->fake_event(new XEvent(event));
}

bool WineWindow::get_passed_event(XEvent& dest)
{
    if (m_passed_events.size())
    {
        std::memcpy(&dest, m_passed_events.front(), sizeof(dest));
        delete m_passed_events.front();
        m_passed_events.pop();
        return true;
    }
    return false;
}

bool WineWindow::on_draw()
{
    unsigned int win_w(get_width()), win_h(get_height());
    
    enter();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, win_w, win_h, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (std::list< Indexable<IDrawSubscriber> >::iterator it = m_draw_subscribers.begin(); it != m_draw_subscribers.end(); it++)
        it->indexed().draw(win_w, win_h);

/*
    glColor4f(1.0f,.0f,.0f, .25f);
    glBegin(GL_QUADS);
    glVertex2f(.25f * win_w, .25f * win_h);
    glVertex2f(.75f * win_w, .25f * win_h);
    glVertex2f(.75f * win_w, .35f * win_h);
    glVertex2f(.25f * win_w, .35f * win_h);
    glEnd();
*/  
    
    leave();
    
    //std::cout << "DebugInfoOutput: " << *(uint32_t*)(Memory::Base + Memory::DebugInfoOutput) << std::endl;
    //*(uint32_t*)(Memory::Base + Memory::DebugInfoOutput) = 1;

    return true;
}

