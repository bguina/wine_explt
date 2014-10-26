
#include <dlfcn.h>

#include "Console.hpp"
#include "UserModuleManager.hpp"

SINGLETON_DECLARE(UserModuleManager);

UserModuleManager::UserModuleManager()
{
    Console& console(Console::module());

    console.add_command("load", *this, &UserModuleManager::handle_command_load);
    console.add_command("unload", *this, &UserModuleManager::handle_command_unload);
    console.add_command("reload", *this, &UserModuleManager::handle_command_reload);
}

UserModuleManager::~UserModuleManager()
{
    for (std::map<std::string const, void*>::iterator it = m_modules.begin(); it != m_modules.end(); ++it)
        unload(it->first);
}

bool UserModuleManager::is_loaded(std::string const& file)
{
    return m_modules.find(file) != m_modules.end();
}

UserModuleManager::LoadResult UserModuleManager::load(std::string const& file, std::string& error)
{
    void *handle;

    if (is_loaded(file))
        return load_conflict;

    if (!(handle = dlopen(file.c_str(), RTLD_NOW)))
    {
        error = dlerror();
        return load_error;
    }

    m_modules[file] = handle;
    return load_success;
}

bool UserModuleManager::unload(std::string const& file)
{
    std::map<std::string const, void*>::iterator it(m_modules.find(file));

    if (it == m_modules.end())
        return false;

    dlclose(it->second);
    m_modules.erase(it);
    return true;
}

//TODO: find more specific command names
//"list" is not registered because too generic
int UserModuleManager::handle_command_list(Console& console, std::vector<std::string>& argv)
{
    (void)argv;
    return handle_command_list(console);
}

int UserModuleManager::handle_command_list(Console& console)
{
    for (std::map<std::string const, void*>::const_iterator it = m_modules.begin(); it != m_modules.end(); ++it)
        console.print(it->first);
    return 0;
}

int UserModuleManager::handle_command_load(Console& console, std::vector<std::string>& argv)
{
    std::string error;

    if (argv.size() != 2)
    {
        console.print("usage: load MODULE_FILE");
        return -1;
    }

    switch (load(argv[1], error))
    {
        case load_success:  console.print("load: module loaded");           return 0;
        case load_conflict: console.print("load: module already loaded");   return 1;
        case load_error:    console.print("laod: error: " + error);         return 2;
        default:            console.print("load: unknown error");           return 3;
    }
}

int UserModuleManager::handle_command_unload(Console& console, std::vector<std::string>& argv)
{
    if (argv.size() != 2)
    {
        console.print("usage: unload MODULE_FILE");
        return -1;
    }

    if (!unload(argv[1]))
    {
       console.print("unload: " + argv[1] + ": no such module");
       handle_command_list(console);
       return 1;
    }

    console.print("unload: success");
    return 0;
}

int UserModuleManager::handle_command_reload(Console& console, std::vector<std::string>& argv)
{
    int result;

    argv[0] = "unload";
    if ((result = handle_command_unload(console, argv)) == 0)
    {
        argv[0] = "load";
        result = handle_command_load(console, argv);
    }
    return result;
}

