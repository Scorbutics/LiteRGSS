#include "CGraphics.h"
#include "common.h"
#include "rbAdapter.h"
#include "Common/Rectangle.h"

#include "ViewportElement.h"
#include "RectangleElement.h"

VALUE rb_cViewport = Qnil;

template<>
void rb::Mark<ViewportElement>(ViewportElement* viewport) {
	if (viewport == nullptr) {
		return;
	}
	rb_gc_mark(viewport->rRect);
	rb_gc_mark(viewport->rTone);
	rb_gc_mark(viewport->rColor);
	rb_gc_mark(viewport->rZ);
	rb_gc_mark(viewport->rAngle);
	rb_gc_mark(viewport->rZoom);
	rb_gc_mark(viewport->rRenderState);
}

VALUE rb_Viewport_Copy(VALUE self) {
	rb_raise(rb_eRGSSError, "Viewports can not be cloned or duplicated.");
	return self;
}

VALUE rb_Viewport_Initialize(int argc, VALUE* argv, VALUE self) {
	/* Viewport setting */
	auto& viewport = rb::Get<ViewportElement>(self);
	viewport.init(CGraphics::Get().addView<cgss::Viewport>());

	/* Creating rect */
	VALUE rc = rb_class_new_instance(argc, argv, rb_cRect);
	auto& rect = rb::Get<RectangleElement>(rc);

	/* Linking Rect */
	viewport->bindRectangle(rect.instance());
	viewport.rRect = rc;
	return self;
}


VALUE rb_Viewport_Dispose(VALUE self) {
	auto& viewport = rb::Get<ViewportElement>(self);
	viewport->dispose();
	return Qnil;
}

VALUE rb_Viewport_getOX(VALUE self) {
	auto& viewport = rb::Get<ViewportElement>(self);
	return rb_int2inum(viewport->getOx());
}

VALUE rb_Viewport_setOX(VALUE self, VALUE val) {
	auto& viewport = rb::Get<ViewportElement>(self);
	viewport->moveOrigin(rb_num2long(val), viewport->getOy());
	return val;
}

VALUE rb_Viewport_getOY(VALUE self) {
	auto& viewport = rb::Get<ViewportElement>(self);
	return rb_int2inum(viewport->getOy());
}

VALUE rb_Viewport_setOY(VALUE self, VALUE val) {
	auto& viewport = rb::Get<ViewportElement>(self);
	viewport->moveOrigin(viewport->getOx(), rb_num2long(val));
	return val;
}

VALUE rb_Viewport_getRect(VALUE self) {
	auto& viewport = rb::Get<ViewportElement>(self);
	return viewport.rRect;
}

VALUE rb_Viewport_setRect(VALUE self, VALUE val) {
	auto& viewport = rb::Get<ViewportElement>(self);

	if (!rb::CheckType<RectangleElement>(self, rb_cRect).empty()) {
		return Qnil;
	}
	
	auto* rect = rb::GetPtr<RectangleElement>(val);	
	if (rect == nullptr || *rect == nullptr) { 
		viewport->bindRectangle(nullptr);
		viewport.rRect = Qnil;
		return Qnil;
	}
	/* Setting rect parameter */
	(*rect)->setRect(viewport->getRectangle());
	viewport->bindRectangle(rect->instance());
	viewport.rRect = val;
	return val;

}

VALUE rb_Viewport_getTone(VALUE self) {
	/*auto& viewport = rb::Get<CViewport_Element>(self);
	VALUE tn = viewport.rTone;
	if (!NIL_P(tn))
		return tn;*/
	/* New tone */
	/*
	VALUE argv[4] = {LONG2FIX(0), LONG2FIX(0), LONG2FIX(0), LONG2FIX(0)};
	viewport.rColor = rb_class_new_instance(4, argv, rb_cColor);
	tn = rb_class_new_instance(4, argv, rb_cTone);
	CTone_Element* tone;
	Data_Get_Struct(tn, CTone_Element, tone);
	tone->bindViewport(&viewport);
	viewport.rTone = tn;
	viewport.create_render();
	return tn;
	*/
	return Qnil;
}

VALUE rb_Viewport_setTone(VALUE self, VALUE val) {
	/*
	VALUE tn = rb_Viewport_getTone(self);
	if (RDATA(tn)->data == nullptr) { return Qnil; }
	if (rb_obj_is_kind_of(val, rb_cTone) != Qtrue) {
		rb_raise(rb_eTypeError, "Expected Tone, got %s", RSTRING_PTR(rb_class_name(CLASS_OF(val))));
		return Qnil;
	}
	if (RDATA(val)->data == nullptr) { return Qnil; }
	auto& viewport = rb::Get<ViewportElement>(self);
	CTone_Element* tonesrc;
	Data_Get_Struct(val, CTone_Element, tonesrc);
	CTone_Element* tonedest;
	Data_Get_Struct(tn, CTone_Element, tonedest);
	sf::Glsl::Vec4* stone = tonesrc->getTone();
	sf::Glsl::Vec4* vtone = viewport.getTone();
	if (vtone->x != stone->x || vtone->y != stone->y || vtone->z != stone->z || vtone->w != vtone->w) {
		tone_copy(tonedest->getTone(), stone);
		tone_copy(vtone, stone);
		viewport.updatetone();
	}*/
	return val;
}

VALUE rb_Viewport_getColor(VALUE self) {
	/*
	VALUE tn = rb_Viewport_getTone(self);
	auto& viewport = rb::Get<ViewportElement>(self);
	return viewport.rColor;
	*/
	return Qnil;
}

VALUE rb_Viewport_setColor(VALUE self, VALUE val) {
	/*
	VALUE tn = rb_Viewport_getTone(self);
	auto& viewport = rb::Get<ViewportElement>(self);
	if(rb_obj_is_kind_of(val, rb_cColor) != Qtrue) {
		rb_raise(rb_eTypeError, "Expected Color, got %s", RSTRING_PTR(rb_class_name(CLASS_OF(val))));
		return Qnil;
	}
	viewport.rColor = val;
	*/
	return self;
}

VALUE rb_Viewport_getVisible(VALUE self) {
	auto& viewport = rb::Get<ViewportElement>(self);
	return viewport->isVisible() ? Qtrue : Qfalse;
}

VALUE rb_Viewport_setVisible(VALUE self, VALUE val) {
	auto& viewport = rb::Get<ViewportElement>(self);
	viewport->setVisible(RTEST(val));
	return self;
}

VALUE rb_Viewport_Update(VALUE self) {
	auto& viewport = rb::Get<ViewportElement>(self);
	// ????
	return self;
}

VALUE rb_Viewport_getZ(VALUE self) {
	auto& viewport = rb::Get<ViewportElement>(self);
	return viewport.rZ;
}

VALUE rb_Viewport_setZ(VALUE self, VALUE val) {
	auto& viewport = rb::Get<ViewportElement>(self);
	const auto z = rb_num2long(val);
	viewport->setZ(z);
	viewport.rZ = val;
	return self;
}

VALUE rb_Viewport_getAngle(VALUE self) {
	auto& viewport = rb::Get<ViewportElement>(self);
	return viewport.rAngle;
}

VALUE rb_Viewport_setAngle(VALUE self, VALUE val) {
	auto& viewport = rb::Get<ViewportElement>(self);
	const auto angle = NUM2LONG(val) % 360;
	viewport.rAngle = LONG2NUM(angle);
	viewport->setAngle(angle);
	return self;
}

VALUE rb_Viewport_getZoom(VALUE self) {
	auto& viewport = rb::Get<ViewportElement>(self);
	return viewport.rZoom;
}

VALUE rb_Viewport_setZoom(VALUE self, VALUE val) {
	auto& viewport = rb::Get<ViewportElement>(self);
	const auto zoom = 1.0 / normalize_double(NUM2DBL(val), 0.001, 1000.0);
	viewport.rZoom = DBL2NUM(zoom);
	viewport->setScale(zoom);
	return self;
}

VALUE rb_Viewport_getRenderState(VALUE self) {
	auto& viewport = rb::Get<ViewportElement>(self);
	return viewport.rRenderState;
}

VALUE rb_Viewport_setRenderState(VALUE self, VALUE val) {
	//TODO
	/*
	sf::RenderStates* render_state;
	auto& viewport = rb::Get<ViewportElement>(self);
	rb_Viewport_getColor(self);
	if (rb_obj_is_kind_of(val, rb_cBlendMode) == Qtrue) {
		Data_Get_Struct(val, sf::RenderStates, render_state);
		if (render_state) {
			viewport.setRenderStates(render_state);
			viewport.rRenderState = val;
			viewport.create_render(); // Make sure the global render is initialized
			viewport.updatetone();
			return self;
		}
	}
	viewport->setRenderStates(nullptr);
	viewport.rRenderState = Qfalse; // False to prevent intempestive delete
	*/
	return self;
}

VALUE rb_Viewport_ReloadStack(VALUE self) {
	// Deprecated
	//auto& viewport = rb::Get<ViewportElement>(self);
	//viewport.syncStackCppFromRuby();
	return self;
}

VALUE rb_Viewport_Index(VALUE self) {
	auto& viewport = rb::Get<ViewportElement>(self);
	return rb_uint2inum(viewport->getZ().index);
}

void Init_Viewport() {
	rb_cViewport = rb_define_class_under(rb_mLiteRGSS, "Viewport", rb_cDrawable);

	rb_define_alloc_func(rb_cViewport, rb::Alloc<ViewportElement>);

	rb_define_method(rb_cViewport, "initialize", _rbf rb_Viewport_Initialize, -1);
	rb_define_method(rb_cViewport, "ox", _rbf rb_Viewport_getOX, 0);
	rb_define_method(rb_cViewport, "ox=", _rbf rb_Viewport_setOX, 1);
	rb_define_method(rb_cViewport, "oy", _rbf rb_Viewport_getOY, 0);
	rb_define_method(rb_cViewport, "oy=", _rbf rb_Viewport_setOY, 1);
	rb_define_method(rb_cViewport, "rect", _rbf rb_Viewport_getRect, 0);
	rb_define_method(rb_cViewport, "rect=", _rbf rb_Viewport_setRect, 1);
	rb_define_method(rb_cViewport, "dispose", _rbf rb_Viewport_Dispose, 0);
	rb_define_method(rb_cViewport, "tone", _rbf rb_Viewport_getTone, 0);
	rb_define_method(rb_cViewport, "tone=", _rbf rb_Viewport_setTone, 1);
	rb_define_method(rb_cViewport, "color", _rbf rb_Viewport_getColor, 0);
	rb_define_method(rb_cViewport, "color=", _rbf rb_Viewport_setColor, 1);
	rb_define_method(rb_cViewport, "update", _rbf rb_Viewport_Update, 0);
	rb_define_method(rb_cViewport, "visible", _rbf rb_Viewport_getVisible, 0);
	rb_define_method(rb_cViewport, "visible=", _rbf rb_Viewport_setVisible, 1);
	rb_define_method(rb_cViewport, "z", _rbf rb_Viewport_getZ, 0);
	rb_define_method(rb_cViewport, "z=", _rbf rb_Viewport_setZ, 1);
	rb_define_method(rb_cViewport, "zoom", _rbf rb_Viewport_getZoom, 0);
	rb_define_method(rb_cViewport, "zoom=", _rbf rb_Viewport_setZoom, 1);
	rb_define_method(rb_cViewport, "angle", _rbf rb_Viewport_getAngle, 0);
	rb_define_method(rb_cViewport, "angle=", _rbf rb_Viewport_setAngle, 1);
	rb_define_method(rb_cViewport, "shader", _rbf rb_Viewport_getRenderState, 0);
	rb_define_method(rb_cViewport, "shader=", _rbf rb_Viewport_setRenderState, 1);
	rb_define_method(rb_cViewport, "blendmode", _rbf rb_Viewport_getRenderState, 0);
	rb_define_method(rb_cViewport, "blendmode=", _rbf rb_Viewport_setRenderState, 1);
	rb_define_method(rb_cViewport, "reload_stack", _rbf rb_Viewport_ReloadStack, 0);
	rb_define_method(rb_cViewport, "__index__", _rbf rb_Viewport_Index, 0);

	rb_define_method(rb_cViewport, "clone", _rbf rb_Viewport_Copy, 0);
	rb_define_method(rb_cViewport, "dup", _rbf rb_Viewport_Copy, 0);
}
