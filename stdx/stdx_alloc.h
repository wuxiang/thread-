#ifndef __STDX_ALLOC_H
#define __STDX_ALLOC_H

// C++ 98 header files
#include <new>      // for placement new
#include <cstddef>  // for ptrdiff_t, size_t
#include <cstdlib>  // for exit
#include <climits>  // for UINT_MAX

namespace stdx {


template <typename _Tp>
inline _Tp*
_allocate(ptrdiff_t size, _Tp*)
{
    std::set_new_handler(0);
    _Tp* tmp = (_Tp*)(::operator new((size_t)(size * sizeof(_Tp))));
    if (tmp == 0)
        exit(1);
    return tmp;
}

template <typename _Tp>
inline void
_deallocate(_Tp* buf)
{
    ::operator delete(buf);
}

template <typename _Tp1, typename _Tp2>
inline void _construct(_Tp1* ptr, const _Tp2& value)
{
    new(ptr) _Tp1(value);
}

template <typename _Tp>
inline void _destroy(_Tp* ptr)
{
    ptr->~_Tp();
}

template<typename _Tp>
class allocator
{
public:
    typedef size_t     size_type;
    typedef ptrdiff_t  difference_type;
    typedef _Tp*       pointer;
    typedef const _Tp* const_pointer;
    typedef _Tp&       reference;
    typedef const _Tp& const_reference;
    typedef _Tp        value_type;

    template<typename _Tp1>
    struct rebind
    { typedef allocator<_Tp1> other; };

    ~allocator() throw() { }

    pointer allocate(size_type n, const void* hint = 0)
    { return _allocate((difference_type)n, (pointer)0); }

    void deallocate(pointer ptr, size_type n)
    { _deallocate(ptr); }

    void construct(pointer ptr, const _Tp& value)
    { _construct(ptr, value); }

    void destroy(pointer ptr)
    { _destroy(ptr); }

    pointer address(reference x)
    { return (pointer)&x; }

    const_pointer const_address(const_reference x)
    { return (const_pointer)&x; }

    size_type max_size() const
    { return size_type(UINT_MAX / sizeof(_Tp)); }
};


} // namespace stdx

#endif // __STDX_ALLOC_H

// vim:set tabstop=4 shiftwidth=4 expandtab:
