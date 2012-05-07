#ifndef __STDX_MEMORY_H
#define __STDX_MEMORY_H


// C 89 header files
#include <stdlib.h>


namespace stdx {


template<typename _Tp>
struct free_functor
{
    free_functor() { }

    template<typename _Up>
    free_functor(const free_functor<_Up>&) { }

    void
    operator()(_Tp* __ptr) const
    {
        free(__ptr);
    }
};

template<typename _Tp>
struct delete_functor
{
    delete_functor() { }

    template<typename _Up>
    delete_functor(const delete_functor<_Up>&) { }

    void
    operator()(_Tp* __ptr) const
    {
        delete __ptr;
    }
};

template<typename _Tp>
struct delete_functor<_Tp[]>
{
    void
    operator()(_Tp* __ptr) const
    {
        delete [] __ptr;
    }
};

template<typename _Tp1>
struct auto_delete_ref
{
    _Tp1* _M_ptr;

    explicit
    auto_delete_ref(_Tp1* __p): _M_ptr(__p) { }
};

//
// template<typename _Tp, typename _Tp_Deleter = delete_functor<_Tp> >
// calling like this: auto_delete<int, free_functor<int> > buf;
//
// use template template parameters
// calling like this: auto_delete<int, free_functor> buf;
//
template<typename _Tp,
         template <typename _Elem> class _Tp_Deleter = delete_functor>
class auto_delete
{
private:
    _Tp* _M_ptr;
    _Tp_Deleter<_Tp> _M_deleter;

public:
    typedef _Tp*               pointer;
    typedef _Tp                element_type;
    typedef _Tp_Deleter<_Tp>   deleter_type;

    explicit
    auto_delete(element_type* __p = 0) throw() : _M_ptr(__p) { }

    auto_delete(auto_delete& __a) throw() : _M_ptr(__a.release()) { }

    template<typename _Tp1>
    auto_delete(auto_delete<_Tp1>& __a) throw() : _M_ptr(__a.release()) { }

    auto_delete&
    operator=(auto_delete& __a) throw()
    {
        reset(__a.release());
        return *this;
    }

    template<typename _Tp1>
    auto_delete&
    operator=(auto_delete<_Tp1>& __a) throw()
    {
        reset(__a.release());
        return *this;
    }

    ~auto_delete() { reset(); }

    element_type&
    operator*() const throw()
    {
        return *_M_ptr;
    }

    element_type*
    operator->() const throw()
    {
        return _M_ptr;
    }

//  _Tp&
//  operator[](size_t __i) const
//  {
//      return get()[__i];
//  }

    element_type*
    get() const throw() { return _M_ptr; }

    const deleter_type&
    get_deleter() const
    { return _M_deleter; }
//  { return _Tp_Deleter(); }

    element_type*
    release() throw()
    {
        element_type* __tmp = _M_ptr;
        _M_ptr = 0;
        return __tmp;
    }

    void
    reset(element_type* __p = 0) throw()
    {
        if (__p != _M_ptr)
        {
            get_deleter()(_M_ptr);
            _M_ptr = __p;
        }
    }

    auto_delete(auto_delete_ref<element_type> __ref) throw()
        : _M_ptr(__ref._M_ptr) { }

    auto_delete&
    operator=(auto_delete_ref<element_type> __ref) throw()
    {
        if (__ref._M_ptr != this->get())
        {
            get_deleter()(_M_ptr);
            _M_ptr = __ref._M_ptr;
        }
        return *this;
    }

    template<typename _Tp1>
    operator auto_delete_ref<_Tp1>() throw()
    { return auto_delete_ref<_Tp1>(this->release()); }

    template<typename _Tp1>
    operator auto_delete<_Tp1>() throw()
    { return auto_delete<_Tp1>(this->release()); }
};


} // namespace stdx


#endif // __STDX_MEMORY_H

// vim:set tabstop=4 shiftwidth=4 expandtab:
