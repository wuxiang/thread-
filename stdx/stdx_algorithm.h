#ifndef __STDX_ALGORITHM_H
#define __STDX_ALGORITHM_H

// C++ 98 header files
#include <algorithm>

namespace stdx {

template <typename _InputIterator1, typename _InputIterator2>
inline bool unordered_includes(_InputIterator1 first1, _InputIterator1 last1,
                               _InputIterator2 first2, _InputIterator2 last2)
{
    typedef typename std::iterator_traits<_InputIterator1>::value_type  _ValueType1;
    typedef typename std::iterator_traits<_InputIterator2>::value_type  _ValueType2;

    for ( ; first2 != last2; ++first2)
    {
        if (std::find(first1, last1, *first2) == last1)
            break;
    }
    return first2 == last2;
}

template <typename _InputIterator1, typename _InputIterator2>
inline bool unordered_match(_InputIterator1 first1, _InputIterator1 last1,
                            _InputIterator2 first2, _InputIterator2 last2)
{
    typedef typename std::iterator_traits<_InputIterator1>::value_type  _ValueType1;
    typedef typename std::iterator_traits<_InputIterator2>::value_type  _ValueType2;

    return unordered_includes(first1, last1, first2, last2)
        && unordered_includes(first2, last2, first1, last1);
}


} // namespace stdx

#endif // __STDX_ALGORITHM_H

// vim:set tabstop=4 shiftwidth=4 expandtab:
