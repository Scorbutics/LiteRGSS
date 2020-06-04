#ifndef Window_H
#define Window_H

#include <LiteCGSS/Views/FramedView.h>
#include "CgssWrapper.h"
#include "RubyValue.h"

extern VALUE rb_cWindow;
void Init_Window();

struct FramedViewElement : 
    public CgssInstance<cgss::FramedView> {

    VALUE rBitmap = Qnil;
    VALUE rX = Qnil;
    VALUE rY = Qnil;
    VALUE rZ = Qnil;
    VALUE rOX = Qnil;
    VALUE rOY = Qnil;
    VALUE rRect = Qnil;
    VALUE rWidth = Qnil;
    VALUE rHeight = Qnil;
    VALUE rCursorRect = Qnil;
    VALUE rBackOpacity = Qnil;
    VALUE rContentOpacity = Qnil;
    VALUE rOpacity = Qnil;
    VALUE rWindowBuilder = Qnil;
    VALUE rPauseSkin = Qnil;
    VALUE rCursorSkin = Qnil;
    VALUE rPause = Qnil;
    VALUE rPauseX = Qnil;
    VALUE rPauseY = Qnil;
    VALUE rActive = Qnil;
    VALUE rStretch = Qnil;
    VALUE rViewport = Qnil;

    template <class Drawable, class ... Args>
    void initAndAdd(Drawable& drawable, Args&& ... args) {
        drawable.init(Drawable::create(*instance(), std::forward<Args>(args)...));
    }
};

#endif
