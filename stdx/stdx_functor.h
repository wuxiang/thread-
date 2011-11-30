#ifndef __STDX_FUNCTOR_H
#define __STDX_FUNCTOR_H

// Posix header files

// C++ 98 header files
#include <functional>


namespace stdx {


template <typename _Pair>
struct select1st_equal_to : std::binary_function<_Pair, typename _Pair::first_type, bool>
{
    typedef typename std::binary_function<_Pair, typename _Pair::first_type, bool>::result_type  result_type;
    result_type operator()(const _Pair& __x, const typename _Pair::first_type& __first) const
    {
        return __x.first == __first;
    }
};

template <typename _Pair>
struct select2nd_equal_to : std::binary_function<_Pair, typename _Pair::second_type, bool>
{
    typedef typename std::binary_function<_Pair, typename _Pair::first_type, bool>::result_type  result_type;
    result_type operator()(const _Pair& __x, const typename _Pair::second_type& __second) const
    {
        return __x.second == __second;
    }
};


} // namespace stdx

#endif // __STDX_FUNCTOR_H

// vim:set tabstop=4 shiftwidth=4 expandtab:
