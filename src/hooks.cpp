
#include <iostream>
#include <cstring>
#include <cassert>

#include <dlfcn.h>

#include <X11/Xlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include "Module.hpp"
#include "UserModuleManager.hpp"
#include "WineWindow.hpp"

// Pretend to be dlsym and lie about glXGetProcAddressARB
static void (*__wineinjection_glXGetProcAddressARB(const GLubyte*))();

extern "C" void *dlsym(void* handle, char const* symbol)
{
    static void* (*real_dlsym)(void*, char const*) = 0;

    if (!real_dlsym)
        assert(real_dlsym = (void* (*)(void*, char const*))dlvsym(RTLD_DEFAULT, "dlsym", "GLIBC_2.0"));
    if (!std::strcmp(symbol, "glXGetProcAddressARB"))
        return (void*)&__wineinjection_glXGetProcAddressARB;
    return real_dlsym(handle, symbol);
}

// Lie about the glXSwapBuffers'
static void __wineinjection_glXSwapBuffers(Display* , GLXDrawable);

static void (*__wineinjection_glXGetProcAddressARB(GLubyte const* proc))()
{
    if (!std::strcmp((const char *)proc, "glXSwapBuffers"))
        return (void (*)())__wineinjection_glXSwapBuffers;
    return glXGetProcAddressARB(proc);
}

// Draw stuff
static void __wineinjection_glXSwapBuffers(Display* dpy, GLXDrawable drawable)
{
    UserModuleManager::module();
    if (WineWindow::module().on_draw())
        glXSwapBuffers(dpy, drawable);
}

// Hack the events
extern "C" Bool XCheckIfEvent(Display *display, XEvent *event_return, Bool (*predicate)(Display*, XEvent*, XPointer), XPointer arg)
{
    static Bool (*orig)(Display*, XEvent*, Bool (*)(Display*, XEvent*, XPointer), XPointer) = 0;
    bool new_event;

    if (!orig)
        assert(orig = (Bool (*)(Display *, XEvent *, Bool (*)(Display*, XEvent*, XPointer), XPointer))dlsym(RTLD_NEXT, "XCheckIfEvent"));
    
    new_event = orig(display, event_return, predicate, arg);
    
    // Is WineWindow ready to hijack XEvents?
    if (WineWindow::module_ptr())
    {
        WineWindow& win(WineWindow::module());
        
        if (new_event)
            win.on_event(*event_return);
        new_event = win.get_passed_event(*event_return);
    }
    
    return new_event;
}

// Unload Modules on the library unload (it may be useful somehow)
std::set<void (*)()> __modules_destroyers;

void __attribute__ ((destructor)) __wineinjection_destructor()
{
    for (std::set<void (*)()>::iterator it = __modules_destroyers.begin(); it != __modules_destroyers.end(); ++it)
        (*it)();
}

