#pragma once

#include "minikit.h"

#define c_class(name) \
/**/	typedef std::shared_ptr<class name> name##Ref; \
/**/	class name
