#ifndef Rect_H
#define Rect_H

#include <LiteCGSS/Common/Rectangle.h>
#include "RubyValue.h"
#include "CgssWrapper.h"

using RectangleElement = CgssInstance<cgss::Rectangle>;
extern VALUE rb_cRect;
void Init_Rect();

#endif
