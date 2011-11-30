#ifndef __STDX_BUFFER_H
#define __STDX_BUFFER_H


// C 89 header files
#include <inttypes.h>
#include <stdlib.h>

// stdx header files
#include "stdx_noncopyable.h"


namespace stdx {

class auto_buffer : private noncopyable
{
public:
    typedef void element_type;
    typedef uint32_t size_type;

public:
    explicit
    auto_buffer(element_type* ptr = 0, size_type size = 0) throw()
        : m_ptr(ptr), m_size(size)
    { }

    ~auto_buffer() { reset(); }

    element_type*
    data() const throw() { return m_ptr; }

    size_type
    size() const throw() { return m_size; }

    void
    reset(element_type* ptr = 0, size_type size = 0) throw()
    {
        if (ptr != m_ptr)
        {
            if (m_ptr != 0)
                free(m_ptr);
            m_ptr = ptr;
            m_size = size;
        }
    }

protected:
    element_type* m_ptr;
    size_type m_size;
};

} // namespace stdx

#endif // __STDX_BUFFER_H

// vim:set tabstop=4 shiftwidth=4 expandtab:
