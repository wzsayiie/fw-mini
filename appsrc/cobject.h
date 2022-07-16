#pragma once

#include "minikit.h"

declare_reflectable_class(CObject)
class d_exportable CObject : public reflect::extends<CObject, MObject> {
};
