
#ifndef HPP_CONSOLE
#define HPP_CONSOLE

#include <string>
#include <vector>
#include <list>
#include <map>

#include "Module.hpp"

class ICommandHandler;

namespace CEGUI
{
    class EventArgs;
    class FrameWindow;
    class MultiLineEditbox;
    class Editbox;
};

class Console : public Module<Console>
{
    friend class Module<Console>;
    
    protected:
    Console();
    virtual ~Console();

    public:
    typedef int (ICommandHandler::*ICommandHandlerMethod)(Console&, std::vector<std::string>&);

    bool is_visible() const;
    void set_visible(bool enable);
    
    bool is_command_registered(std::string const& command) const;
    template<class T> bool add_command(std::string const& command, ICommandHandler& handler, T method)
    {
        return add_command<ICommandHandlerMethod>(command, handler, static_cast<ICommandHandlerMethod>(method));
    }
    bool remove_command(std::string const& command);
    
    void print(std::string const& msg="\n");
    bool execute(std::string const& command);
    
    protected:
    static unsigned int const history_limit;
    
    void _history_add(std::string const& input);
    bool _on_input_keydown(CEGUI::EventArgs const& e);
    bool _on_input_validation(CEGUI::EventArgs const& e);
    
    std::map< std::string const, std::pair< ICommandHandler&, ICommandHandlerMethod> > m_commands;
    
    std::list<std::string> m_history;
    std::list<std::string>::iterator m_history_cur;

    CEGUI::FrameWindow* m_frame;
    CEGUI::MultiLineEditbox* m_content;
    CEGUI::Editbox* m_input;
};

template<> bool Console::add_command(std::string const& command, ICommandHandler& handler, ICommandHandlerMethod method);

#endif

