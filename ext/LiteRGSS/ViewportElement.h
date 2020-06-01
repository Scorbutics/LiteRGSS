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

    template <class Drawable, class ... Args>
    void initAndAdd(Drawable& drawable, Args&& ... args) {
        drawable.init(Drawable::create(*instance()), std::forward<Args>(args)...);
    }
};

namespace rb {
	template<>
	void Mark<ViewportElement>(ViewportElement* sprite);
}

#endif