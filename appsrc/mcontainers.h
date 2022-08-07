#include "reflect.h"

template<class F>          using MFunction = reflect::function<F>;
template<class V>          using MVector   = reflect::vector<V>;
template<class K, class V> using MMap      = reflect::map<K, V>;
template<class V>          using MSet      = reflect::set<V>;

using MBaseFunction = reflect::base_function;
using MBaseVector   = reflect::base_vector  ;
using MBaseMap      = reflect::base_map     ;
using MBaseSet      = reflect::base_set     ;

#define MF reflect::make_function
