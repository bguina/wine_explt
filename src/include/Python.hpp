
#ifndef HPP_PYTHON
#define HPP_PYTHON

#include "Module.hpp"

class Python : public Module<Python>
{
    friend class Module<Python>;

    protected:
    Python();
    virtual ~Python();
};

#endif

