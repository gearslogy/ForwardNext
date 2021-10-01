//
// Created by liuyangping on 2021/10/1.
//

#ifndef NODE_AND_ITEMS_PCG_CONCEPT_HPP
#define NODE_AND_ITEMS_PCG_CONCEPT_HPP

#include <concepts>


template<typename X>
using access_value_type  =  typename X::value_type;
template<typename X>
using access_iterator_of =  typename X::iterator;

template< class T >
concept is_range = requires(T t) {
    typename access_value_type<T>;    // T must have value_type
    typename access_iterator_of<T>;   // T must have iterator
    t.begin() ;
    t.end();
};

template <class T>
concept is_not_range = not is_range<T> or std::is_same_v<T, std::string>; // do not support string type



#endif //NODE_AND_ITEMS_PCG_CONCEPT_HPP
