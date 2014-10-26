
#ifndef HPP_INDEXABLE
#define HPP_INDEXABLE

#include <cassert>

#include "Indexable.hpp"

template<class T>
class Indexable
{
    public:
    Indexable(T* indexed, unsigned int index=0)
        : m_indexed(indexed), m_index(index)
    {
        assert(m_indexed);
    }

    T& indexed()
    {
        return *m_indexed;
    }
    
    bool operator<(Indexable const& indexed) const
    {
        return m_index < indexed.m_index;
    }

    bool operator==(Indexable const& indexed) const
    {
        return Indexable<T>::m_indexed == indexed.m_indexed;
    }

    protected:
    T* m_indexed;
    unsigned int m_index;
};

#endif

