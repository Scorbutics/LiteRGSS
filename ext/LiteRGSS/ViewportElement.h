#ifndef ViewportElement_H
#define ViewportElement_H

#include "CgssWrapper.h"
#include "Views/Viewport.h"

struct ViewportElement : 
    public CgssInstance<cgss::Viewport> {
        
    VALUE rRect = Qnil;
    VALUE rTone = Qnil;
    VALUE rColor = Qnil;
	VALUE rZ = LONG2FIX(0);
	VALUE rAngle = LONG2FIX(0);
    VALUE rZoom = LONG2FIX(1);
    VALUE rRenderState = Qnil;
};

#endif