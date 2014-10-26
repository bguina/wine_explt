
#ifndef HPP_GUI
#define HPP_GUI

#include <map>

#include <CEGUI/CEGUI.h>

#include "Module.hpp"
#include "IEventSubscriber.hpp"
#include "IDrawSubscriber.hpp"

class GUI : public Module<GUI>, public IEventSubscriber, public IDrawSubscriber
{
    friend class Module<GUI>;

    protected:
    GUI();
    virtual ~GUI();

    public:
    void add_child(CEGUI::Window* child);
    void remove_child(CEGUI::Window* child);
    
    // Interface for WineWindow
    virtual void draw(unsigned int win_w, unsigned int win_h);
    virtual bool inject_event(XEvent& event);
    
    protected:
    CEGUI::Window* m_root;
    std::map<unsigned int, unsigned int> m_keyscans;
};

#endif

