
#ifndef HPP_MODULE
#define HPP_MODULE

// Used when the library is unloaded to destroy the modules
#include <set>
extern std::set<void (*)()> __modules_destroyers;

#include <cassert>

template<class T>
class Module
{
    public:
    static T* module_ptr()
    {
        return m_singleton;
    }

    static T& module()
    {
        if (!m_singleton)
        {
            // Two (or more) singletons must NOT construct each other (or may the recursion pity you)
            assert(!m_singleton_created);
            assert(!m_singleton_destroyed);
            m_singleton_created = true;
            m_singleton = new T();
            __modules_destroyers.insert(&_destroy);
        }
        return *m_singleton;
    }

    protected:
    virtual ~Module() {}
    
    private:
    static void _destroy()
    {
        m_singleton_destroyed = true;
        delete m_singleton;
        m_singleton = 0;
    }
    static bool m_singleton_created;
    static bool m_singleton_destroyed;
    static T* m_singleton;
};

#define SINGLETON_DECLARE(name)\
template<> bool Module<name>::m_singleton_created = false;\
template<> bool Module<name>::m_singleton_destroyed = false;\
template<> name* Module<name>::m_singleton = 0

#endif

