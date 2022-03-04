#pragma once

#include "reflect.h"

//pixel scale:
//

void  MSetPixelScale(float scale);
float MGetPixelScale();

#define pt_from_px _MPointFromPixelCaster()<<
#define px_from_pt _MPixelFromPointCaster()<<

struct _MPointFromPixelCaster { float operator<<(float px); };
struct _MPixelFromPointCaster { float operator<<(float pt); };
