#ifndef __STDX_ALGORITHM_H
#define __STDX_ALGORITHM_H

// C++ 98 header files
#include <algorithm>

namespace stdx {

// Exchange sorts:
// Bubble sort · Cocktail sort · Odd-even sort · Comb sort · Gnome sort · Quicksort

template <typename _BidirectionalIterator>
inline void
bubble_sort(_BidirectionalIterator __first, _BidirectionalIterator __last)
{
    bool swapped = true;
    while (swapped && __first != __last)
    {
        swapped = false;
        --__last;
        for (_BidirectionalIterator __next = __first, __cur = __next++;
                __cur != __last; ++__next)
        {
            if (*__next < *__cur)
            {
                std::iter_swap(__next, __cur);
                swapped = true;
            }
            __cur = __next;
        }
    }
}

template <typename _BidirectionalIterator>
inline void
cocktail_sort(_BidirectionalIterator __first, _BidirectionalIterator __last)
{
    --__last;
    bool swapped = false;
    do
    {
        swapped = false;
        for (_BidirectionalIterator __next = __first, __cur = __next++;
                __cur != __last; ++__next)
        {
            if (*__next < *__cur)
            {
                std::iter_swap(__next, __cur);
                swapped = true;
            }
            __cur = __next;
        }
        if (!swapped)
            break;

        swapped = false;
        for (_BidirectionalIterator __prev = __last, __cur = __prev--;
                __cur != __first; --__prev)
        {
            if (*__cur < *__prev)
            {
                std::iter_swap(__cur, __prev);
                swapped = true;
            }
            __cur = __prev;
        }
    } while (swapped);
}

// http://en.wikipedia.org/wiki/Odd-even_sort
template <typename _ForwardIterator>
inline void
oddeven_sort(_ForwardIterator __first, _ForwardIterator __last)
{
    bool swapped = true;
    while (swapped)
    {
        swapped = false;

        for (_ForwardIterator __second = __first, __next = ++__second, __cur = __next++;
                __cur != __last && __next != __last; ++__next, __cur = __next++)
        {
            if (*__next < *__cur)
            {
                std::iter_swap(__next, __cur);
                swapped = true;
            }
        //  std::advance(__cur, 2);
        //  std::advance(__next, 2);
        }

        for (_ForwardIterator __next = __first, __cur = __next++;
                __cur != __last && __next != __last; )// ++__next, __cur = __next++)
        {
            if (*__next < *__cur)
            {
                std::iter_swap(__next, __cur);
                swapped = true;
            }
            std::advance(__cur, 2);
            std::advance(__next, 2);
        }
    }
}

template <typename _ForwardIterator>
inline void
comb_sort(_ForwardIterator __first, _ForwardIterator __last)
{
    static const double shrink_factor = 1.247330950103979;
    typedef typename std::iterator_traits<_ForwardIterator>::difference_type _DistanceType;
    _DistanceType gap = std::distance(__first, __last);
    bool swapped = true;

    while (gap > 1 || swapped)
    {
        if (gap > 1)
            gap = static_cast<_DistanceType>(gap / shrink_factor);

        swapped = false;
        _ForwardIterator __cur(__first);
        _ForwardIterator __next(__first);
        for (std::advance(__next, gap) ; __next != __last; ++__cur, ++__next)
        {
            if (*__next < *__cur)
            {
                std::iter_swap(__cur, __next);
                swapped = true;
            }
        }
    }
}

template <typename _BidirectionalIterator>
inline void
gnome_sort(_BidirectionalIterator __first, _BidirectionalIterator __last)
{
    for (_BidirectionalIterator __second = __first, __prev = ++__second, __cur = __prev--;
            __cur != __last; )
    {
        if (*__prev < *__cur)
        {
            __prev = __cur++;
        }
        else
        {
            std::iter_swap(__cur, __prev);
            if (__cur != __second)
                __cur = __prev--;
        }
    }

//  _BidirectionalIterator pos = first+1;
//  while (pos < last) {
//      if (*pos >= *(pos-1))
//          ++pos;
//      else {
//          std::iter_swap(pos, pos-1);
//          if (pos > (first+1))
//              --pos;
//      }
//  }
}

template <typename _ForwardIterator>
inline void
selection_sort(_ForwardIterator __first, _ForwardIterator __last)
{
    for (_ForwardIterator __i = __first; __i != __last; ++__i)
    {
        _ForwardIterator __min = __i;
        for (_ForwardIterator __j = __i; __j != __last; ++__j)
        {
            if (*__j < *__min)
                __min = __j;
        }
        std::iter_swap(__i, __min);
    }
}

template <typename _BidirectionalIterator>
inline void
insertion_sort(_BidirectionalIterator __first, _BidirectionalIterator __last)
{
    typedef typename std::iterator_traits<_BidirectionalIterator>::value_type  _ValueType;
    _BidirectionalIterator __i = __first;
    for (++__i; __i != __last; ++__i)
    {
        _ValueType __val = *__i;
        _BidirectionalIterator __j = __first;
        _BidirectionalIterator __end = __i;
        for (++__end ; __j != __i && *__j < __val; ++__j);
        std::copy_backward(__j, __i, __end);
        *__j = __val;
    }
}

/*
void shell_sort(int array[], int N)
{
    for (int gap = N/2; gap > 0; gap /= 2)
        for (int i = gap; i < N; ++i)
            for (int j = i-gap; j >= 0 && v[j] > v[j+gap]; j -= gap)
                swap(v[j], v[j+gap]);
}
*/

template <typename _BidirectionalIterator>
inline void
shell_sort(_BidirectionalIterator __first, _BidirectionalIterator __last)
{
    typedef typename std::iterator_traits<_BidirectionalIterator>::value_type  _ValueType;
    typedef typename std::iterator_traits<_BidirectionalIterator>::difference_type _DistanceType;
    const double shrink_factor = 2.2;
    const _DistanceType N = std::distance(__first, __last);

    for (_DistanceType gap = N / 2; gap > 0; gap /= shrink_factor)
    {
        /*
        _BidirectionalIterator __i = __first;
        for (std::advance(__i, gap); __i != __last; ++__i)
        {
            _BidirectionalIterator __start = __first;
            std::advance(__start, std::distance(__first, __i) % gap);

            _BidirectionalIterator __cur = __i;
            _BidirectionalIterator __prev = __i;
            std::advance(__prev, -gap);
            while (__cur != __start && *__cur < *__prev)
            {
                std::iter_swap(__cur, __prev);
                __cur = __prev;
                std::advance(__prev, -gap);
            }
        }
        */

        for (_DistanceType i = gap; i < N; ++i)
        {
            _BidirectionalIterator __start = __first;
            std::advance(__start, i % gap);

            _BidirectionalIterator __cur = __first;
            std::advance(__cur, i);
            _BidirectionalIterator __prev = __cur;
            std::advance(__prev, -gap);

            while (__cur != __start && *__cur < *__prev)
            {
                std::iter_swap(__cur, __prev);
                __cur = __prev;
                std::advance(__prev, -gap);
            }
        }
    }
}

template <typename _FlagType>
inline void set_flag(_FlagType& flags, _FlagType flag)
{
    flags |= flag;
}

template <typename _FlagType>
inline void clear_flag(_FlagType& flags, _FlagType flag)
{
    flags &= ~flag;
}

template <typename _FlagType>
inline bool has_flag(_FlagType flags, _FlagType flag)
{
    return (flag & flags) != 0;
}

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
