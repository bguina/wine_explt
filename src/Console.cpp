
#include <iostream>
#include <cstdlib>

#include <CEGUI/CEGUI.h>

#include "WineWindow.hpp"
#include "GUI.hpp"
#include "ICommandHandler.hpp"
#include "str2av.h"
#include "Console.hpp"

SINGLETON_DECLARE(Console);

unsigned int const Console::history_limit = 200;

Console::Console()
    : m_history_cur(m_history.end())
{
    GUI& sheet(GUI::module());
    CEGUI::WindowManager& winmgr(CEGUI::WindowManager::getSingleton());

    WineWindow::module().enter();
    
    m_frame = static_cast<CEGUI::FrameWindow*>(winmgr.createWindow("TaharezLook/FrameWindow"));
    m_frame->setPosition(CEGUI::UVector2(CEGUI::UDim(.0f, .0f), CEGUI::UDim(.0f, .0f)));
    m_frame->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f, .0f), CEGUI::UDim(.33f, .0f)));
    m_frame->setText("ZuberWine Console");
    sheet.add_child(m_frame);
    
    m_content = static_cast<CEGUI::MultiLineEditbox*>(winmgr.createWindow("TaharezLook/MultiLineEditbox"));
    m_content->setPosition(CEGUI::UVector2(CEGUI::UDim(.0f, .0f), CEGUI::UDim(.0f, .0f)));
    m_content->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f, .0f), CEGUI::UDim(1.0f, -32.0f)));
    m_content->setReadOnly(true);
    m_content->setText("Console created\n");
    m_frame->addChildWindow(m_content);

    m_input = static_cast<CEGUI::Editbox*>(winmgr.createWindow("TaharezLook/Editbox"));
    
    m_input->subscribeEvent(CEGUI::Editbox::EventKeyDown, CEGUI::Event::Subscriber(&Console::_on_input_keydown, this));
    m_input->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&Console::_on_input_validation, this));
    m_input->setPosition(CEGUI::UVector2(CEGUI::UDim(.0f, .0f), CEGUI::UDim(1.0f, -32.0f)));
    m_input->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f, .0f), CEGUI::UDim(0.0f, 32.0f)));
    m_frame->addChildWindow(m_input);
    
    WineWindow::module().leave();
}

Console::~Console()
{
    //TODO
}

bool Console::is_visible() const
{
    return m_frame->isVisible();
}

void Console::set_visible(bool enable)
{
    m_frame->setVisible(enable);
}

void Console::print(std::string const& msg)
{
    CEGUI::Scrollbar& scrollbar(*m_content->getVertScrollbar());
    float pos(scrollbar.getScrollPosition());
    bool stick_down(pos >= scrollbar.getDocumentSize() - scrollbar.getPageSize());

    m_content->appendText(msg);
    scrollbar.setScrollPosition((stick_down) ? scrollbar.getDocumentSize() - scrollbar.getPageSize() : pos);
}

bool Console::is_command_registered(std::string const& command) const
{
    return (m_commands.find(command) != m_commands.end());
}

template<> bool Console::add_command(std::string const& command, ICommandHandler& handler, ICommandHandlerMethod method)
{
    return m_commands.insert(std::pair<std::string const, std::pair< ICommandHandler&, ICommandHandlerMethod> >(
            command,
            std::pair<ICommandHandler&, ICommandHandlerMethod>(
                handler,
                method
            )
        )).second;
}

bool Console::remove_command(std::string const& command)
{
    return m_commands.erase(command);
}   

bool Console::execute(std::string const& commandline)
{
    int argc;
    char** argv;
    bool handled(false);

    print("> " + commandline+'\n');

    if (str2av(commandline.c_str(), &argc, &argv) == STR2AV_OK)
    {
        if (argc)
        {
            std::vector<std::string> args_vector;
            std::map< std::string const, std::pair<ICommandHandler&, ICommandHandlerMethod> >::iterator command;
            
            args_vector.reserve(argc);
            for (int i = 0; i < argc; i++)
                args_vector.push_back(argv[i]);
            free(argv[0]);
            if ((command = m_commands.find(args_vector[0])) != m_commands.end())
            {
                (command->second.first.*command->second.second)(*this, args_vector);
                handled = true;
            }
            else
                print(args_vector[0] + ": command not found");
        }
        free(argv);
    }
    else
        print(commandline + ": unbalanced quotes");
    return handled;
}

void Console::_history_add(std::string const& input)
{
    if (m_history.size() == history_limit)
       m_history.pop_front();
    m_history.push_back(input);
}

bool Console::_on_input_keydown(CEGUI::EventArgs const& e)
{
    CEGUI::Key::Scan const& key = static_cast<CEGUI::KeyEventArgs const&>(e).scancode;
    bool consumed(false);

    switch (key)
    {
        case CEGUI::Key::ArrowUp:
        if (m_history_cur != m_history.begin())
        {   // Set the previous history entry (older)
            if (m_history_cur == m_history.end())
            {   // Save the current input in the history
                _history_add(std::string(m_input->getText().c_str()));
                m_history_cur = m_history.end();
                --m_history_cur;
            }
            --m_history_cur;
            m_input->setText(*m_history_cur);
            m_input->setCaratIndex(-1);
            consumed = true;
        }
        break;
        case CEGUI::Key::ArrowDown:
        if (m_history_cur != m_history.end())
        {   // Set the next history entry (newer, or "current" if last)
            m_history_cur++;
            m_input->setText(*m_history_cur);
            m_input->setCaratIndex(-1);

            if (++m_history_cur == m_history.end())
            {   // We just restored the "current" input, erase it from the history
                m_history.erase(--m_history_cur);
                m_history_cur = m_history.end();
            }
            else
                --m_history_cur;

            consumed = true;
        }
        break;
        default: ;
    }
    return consumed;
}

bool Console::_on_input_validation(CEGUI::EventArgs const& e)
{
    std::string commandline(m_input->getText().c_str());

    // We know the window is m_input so it is of no use
    (void)e;

    // If the user was using the history, erase the previous "current" input
    if (m_history_cur != m_history.end())
    {
        m_history.pop_back();
        m_history_cur = m_history.end();
    }

    // Never make the history repeat itself, as my history teacher told me (also, ignore empty strings)
    if (commandline != m_history.back() && commandline.find_first_not_of(' ') != std::string::npos)
        _history_add(m_input->getText().c_str());

    execute(std::string(m_input->getText().c_str()));
    m_input->setText("");
    return true;
}

