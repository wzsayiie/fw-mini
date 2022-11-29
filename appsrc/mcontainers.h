#include "reflect.h"

template<class F>          using MFunction = reflect::function<F>;
template<class V>          using MVector   = reflect::vector<V>;
template<class K, class V> using MMap      = reflect::map<K, V>;
template<class V>          using MSet      = reflect::set<V>;

using MGenericFunction = reflect::generic_function;
using MGenericVector   = reflect::generic_vector  ;
using MGenericMap      = reflect::generic_map     ;
using MGenericSet      = reflect::generic_set     ;

#define MF reflect::make_function
