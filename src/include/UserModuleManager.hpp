
#ifndef H_USERMODULEMANAGER
#define H_USERMODULEMANAGER

#include <map>
#include <vector>
#include <string>

#include "Module.hpp"
#include "ICommandHandler.hpp"

class Console;

class UserModuleManager : public Module<UserModuleManager>, public ICommandHandler
{
    friend class Module<UserModuleManager>;

    protected:
    UserModuleManager();
    virtual ~UserModuleManager();

    public:
    typedef enum
    {
        load_success=0,
        load_conflict,
        load_error,
    } LoadResult;

    bool is_loaded(std::string const& file);
    UserModuleManager::LoadResult load(std::string const& file, std::string& error);
    bool unload(std::string const& file);

    protected:
    int handle_command_list(Console& console, std::vector<std::string>& argv);
    int handle_command_list(Console& console);
    int handle_command_load(Console& console, std::vector<std::string>& argv);
    int handle_command_unload(Console& console, std::vector<std::string>& argv);
    int handle_command_reload(Console& console, std::vector<std::string>& argv);
   
    std::map<std::string const, void*> m_modules;
};

#endif

