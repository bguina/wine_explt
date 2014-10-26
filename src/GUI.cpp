
#include <iostream>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/CEGUIOpenGLRenderer.h>

#include "WineWindow.hpp"
#include "x11keysym2ucs.h"
#include "GUI.hpp"

SINGLETON_DECLARE(GUI);

GUI::GUI()
    : m_root(0)
{
    WineWindow& win(WineWindow::module());
    
    // Register the module for events and drawing
    win.add_event_subscriber(this, WineWindow::event_subscription_max_priority);
    win.add_draw_subscriber(this, WineWindow::draw_subscription_max_priority);

    {   // Unrepresentable keys conversion from X11 (X11/keysymdef.h) to CEGUI
#define MAP_CEGUI_SCAN(sym, scan) m_keyscans[sym] = CEGUI::Key::scan
        MAP_CEGUI_SCAN(XK_BackSpace, Backspace);
        MAP_CEGUI_SCAN(XK_Tab, Tab);
        MAP_CEGUI_SCAN(XK_Linefeed, Return);
        //XK_Clear 
        MAP_CEGUI_SCAN(XK_Return, Return);
        MAP_CEGUI_SCAN(XK_Pause, Pause);
        MAP_CEGUI_SCAN(XK_Scroll_Lock, ScrollLock);
        MAP_CEGUI_SCAN(XK_Sys_Req, SysRq);
        MAP_CEGUI_SCAN(XK_Escape, Escape);
        MAP_CEGUI_SCAN(XK_Delete, Delete);
        MAP_CEGUI_SCAN(0x007f, Delete); // Pressing Delete gives 0x007f and not XK_Delete for me (benoit.g)
        MAP_CEGUI_SCAN(XK_Home, Home);
        MAP_CEGUI_SCAN(XK_Left, ArrowLeft);
        MAP_CEGUI_SCAN(XK_Up, ArrowUp);
        MAP_CEGUI_SCAN(XK_Right, ArrowRight);
        MAP_CEGUI_SCAN(XK_Down, ArrowDown);
        //XK_Prior
        MAP_CEGUI_SCAN(XK_Page_Up, PageUp);
        //XK_Next
        MAP_CEGUI_SCAN(XK_Page_Down, PageDown);
        MAP_CEGUI_SCAN(XK_End, End);
        MAP_CEGUI_SCAN(XK_Begin, Home);
        //XK_Select
        //XK_Print
        //XK_Execute
        MAP_CEGUI_SCAN(XK_Insert, Insert);
        //XK_Undo
        //XK_Redo
        //XK_Menu
        //XK_Find
        //XK_Cancel
        //XK_Help
        //XK_Break
        //XK_Mode_switch
        MAP_CEGUI_SCAN(XK_Num_Lock, NumLock);
        MAP_CEGUI_SCAN(XK_KP_Enter, NumpadEnter);
        MAP_CEGUI_SCAN(XK_F1, F1);
        MAP_CEGUI_SCAN(XK_F2, F2);
        MAP_CEGUI_SCAN(XK_F3, F3);
        MAP_CEGUI_SCAN(XK_F4, F4);
        MAP_CEGUI_SCAN(XK_F5, F5);
        MAP_CEGUI_SCAN(XK_F6, F6);
        MAP_CEGUI_SCAN(XK_F7, F7);
        MAP_CEGUI_SCAN(XK_F8, F8);
        MAP_CEGUI_SCAN(XK_F9, F9);
        MAP_CEGUI_SCAN(XK_F10, F10);
        MAP_CEGUI_SCAN(XK_F11, F11);
        MAP_CEGUI_SCAN(XK_F12, F12);
        MAP_CEGUI_SCAN(XK_F13, F13);
        MAP_CEGUI_SCAN(XK_F14, F14);
        MAP_CEGUI_SCAN(XK_F15, F15);
        MAP_CEGUI_SCAN(XK_Shift_L, LeftShift);
        MAP_CEGUI_SCAN(XK_Shift_R, RightShift);
        MAP_CEGUI_SCAN(XK_Control_L, LeftControl);
        MAP_CEGUI_SCAN(XK_Control_R, RightControl);
        MAP_CEGUI_SCAN(XK_Alt_L, LeftAlt);
        MAP_CEGUI_SCAN(XK_Alt_R, RightAlt);
        MAP_CEGUI_SCAN(XK_Super_L, LeftWindows);
        MAP_CEGUI_SCAN(XK_Super_R, RightWindows);
#undef MAP_CEGUI_SCAN
    }

    {   // CEGUI setup begin
        win.enter();    // CEGUI expects its opengl context to be current
        
        CEGUI::MouseCursor::setInitialMousePosition(CEGUI::Point(-1.0f, -1.0f));
        CEGUI::OpenGLRenderer::bootstrapSystem();

        {   // Specify the default resources' folders
            CEGUI::DefaultResourceProvider* rp;

            rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
            rp->setResourceGroupDirectory("schemes", "/usr/local/share/CEGUI/schemes/");
            rp->setResourceGroupDirectory("imagesets", "/usr/local/share/CEGUI/imagesets/");
            rp->setResourceGroupDirectory("fonts", "/usr/local/share/CEGUI/fonts/");
            rp->setResourceGroupDirectory("layouts", "/usr/local/share/CEGUI/layouts/");
            rp->setResourceGroupDirectory("looknfeels", "/usr/local/share/CEGUI/looknfeel/");
            rp->setResourceGroupDirectory("lua_scripts", "/usr/local/share/CEGUI/lua_scripts/");
        
            CEGUI::Imageset::setDefaultResourceGroup("imagesets");
            CEGUI::Font::setDefaultResourceGroup("fonts");
            CEGUI::Scheme::setDefaultResourceGroup("schemes");
            CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
            CEGUI::WindowManager::setDefaultResourceGroup("layouts");
            CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
        }

        {   // Choose the default style and the default font
            CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
            CEGUI::FontManager::getSingleton().create("DejaVuSans-10.font");
        }

        {   // Setup the GUI sheet root window
            CEGUI::System& sys(CEGUI::System::getSingleton());
            CEGUI::WindowManager& winmgr(CEGUI::WindowManager::getSingleton());
            
            m_root = static_cast<CEGUI::Window*>(winmgr.createWindow("DefaultWindow", "root"));
            m_root->setPosition(CEGUI::UVector2(CEGUI::UDim(.0f, .0f), CEGUI::UDim(.0f, .0f)));
            m_root->setMousePassThroughEnabled(true);
            sys.setGUISheet(m_root);
/*
            // Additional widgets
            CEGUI::FrameWindow* fWnd;
            
            fWnd = static_cast<CEGUI::FrameWindow*>(winmgr.createWindow("TaharezLook/FrameWindow", "hello"));
            root->addChildWindow(fWnd);
            fWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(.25f, .0f), CEGUI::UDim(0.25f, 0)));
            fWnd->setSize(CEGUI::UVector2(CEGUI::UDim(.5f, .0f), CEGUI::UDim(.5f, 0)));
            fWnd->setText("Hello World!");

            CEGUI::MultiLineEditbox* test;

            test = static_cast<CEGUI::MultiLineEditbox*>(winmgr.createWindow("TaharezLook/MultiLineEditbox", "test"));
            fWnd->addChildWindow(test);
            test->setPosition(CEGUI::UVector2(CEGUI::UDim(.25f, .0f), CEGUI::UDim(0.25f, 0)));
            test->setSize(CEGUI::UVector2(CEGUI::UDim(.5f, .0f), CEGUI::UDim(.5f, 0)));
            test->setText("this is a\nmultiMOTHERFUCKINGline\neditbox widget!");
        */
        }
        win.leave();    // Restore the game's opengl context
    }   // CEGUI setup end
}

GUI::~GUI()
{
    //TODO
}

void GUI::add_child(CEGUI::Window* child)
{
    m_root->addChildWindow(child);
}

void GUI::remove_child(CEGUI::Window* child)
{
    m_root->removeChildWindow(child);
}

void GUI::draw(unsigned int win_w, unsigned int win_h)
{
    CEGUI::System& sys(CEGUI::System::getSingleton());
   
    (void)(win_w + win_h);
    sys.injectTimePulse(.33f);//TODO assuming 33fps for now
    sys.renderGUI();
}

bool GUI::inject_event(XEvent& event)
{
    static CEGUI::MouseButton button_map[Button5 + 1] =
    {   // Button5 == 5 is documented
        CEGUI::NoButton,
        CEGUI::LeftButton,
        CEGUI::MiddleButton,
        CEGUI::RightButton,
        CEGUI::X1Button,
        CEGUI::X2Button,
    };
    CEGUI::System& sys(CEGUI::System::getSingleton());
    bool consumed(false);

    // CEGUI expects its opengl context to be current
    WineWindow::module().enter();

    switch (event.type)
    {
        // Keyboard-related
        case KeyPress:
        case KeyRelease:
        {
            KeySym sym;
            std::map<unsigned int, unsigned int>::const_iterator it;

            XLookupString(&event.xkey, 0, 0, &sym, 0);

            {   // Convert keypad keysyms into "standard" representable symkeys
                // For some reason, XK_KP_Enter is the only keypad key CEGUI seems to handle
/*                if (sym >= XK_KP_0 && sym <= XK_KP_9)
                    sym += XK_0 - XK_KP_0;
                else if (sym >= XK_KP_Space && sym <= XK_KP_Enter)
                    sym += XK_space - XK_KP_Space;
                else if (sym >= XK_KP_F1 && sym <= XK_KP_F4)
                    sym += XK_F1 - XK_KP_F1;
                else if (sym >= XK_KP_Home && sym <= XK_KP_Begin)
                    sym += XK_Home - XK_KP_Home;
                else if (sym == XK_KP_Insert)
                    sym = XK_Insert;
                else if (sym == XK_KP_Delete)
                    sym = XK_Delete;
                else if (sym >= XK_KP_Multiply && XK_KP_Subtract)
                    sym += XK_asterisk - XK_KP_Multiply;
                else if (sym == XK_KP_Divide)
                    sym = XK_slash;
*/            } //FIXME Bug Makes ctrl, alt, shift -> a, e, c etc.
            

            if ((it = m_keyscans.find(sym)) != m_keyscans.end())
            {   // Inject a non-representable key (a "control" key)
                consumed = (sys.*((event.type == KeyPress) ? &CEGUI::System::injectKeyDown : &CEGUI::System::injectKeyUp))(it->second);
            }
            else if (event.type == KeyPress)
            {   // Inject a representable key (a character key)
                unsigned int unicode;

                unicode = x11keysym2ucs(sym);
                if (unicode == (unsigned int)-1)
                    std::cerr << "no unicode value for keysym " << (void*)sym << std::endl;
                else
                {
                    consumed = sys.injectChar(unicode);
                }
            }
        }
        break;

        // Mouse-related
        case ButtonPress:   consumed = sys.injectMouseButtonDown(button_map[event.xbutton.button]); break;
        case ButtonRelease: consumed = sys.injectMouseButtonUp(button_map[event.xbutton.button]);   break;
        case MotionNotify:
        sys.injectMousePosition(event.xmotion.x, event.xmotion.y);
        break;

        // Window-properties-related
        case ConfigureNotify: // Window size/position/border/stacking order change
        sys.notifyDisplaySizeChanged(CEGUI::Size(event.xconfigure.width, event.xconfigure.height));
        m_root->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f, .0f), CEGUI::UDim(1.0f, .0f)));
        break;

#if 0   // ignored events
        case KeymapNotify:      std::cout << "keymap\n";break;      // Which keys are pressed
        case MappingNotify:     std::cout << "mapping\n";break;     // The keyboard mapping (modifiers/keysyms/...) has changed
        case EnterNotify:       std::cout << "enter\n";break;       // The cursor enters the window (sent twice)
        case LeaveNotify:       std::cout << "leave\n";break;       // The cursor leaves the window (NEVER triggered)
        case Expose:            std::cout << "expose\n";break;      // The window is moving
        case FocusIn:           std::cout << "focusin\n";break;     // The window got the focus
        case FocusOut:          std::cout << "focusout\n";break;    // The window lost the focus
        case MapNotify:         std::cout << "map\n";break;         // Window shown (or unminimized)
        case UnmapNotify:       std::cout << "unmap\n";break;       // Window hidden (minimized)
        case PropertyNotify:    std::cout << "property\n";break;    // Window property change (background/foreground/taskbar hover/...)
        case ClientMessage:     std::cout << "clientmsg\n";break;   // Custom protocol data? unknown
#endif

        default: ;
    }

    // Restore the game's opengl context
    WineWindow::module().leave();
    
    return consumed;
}

