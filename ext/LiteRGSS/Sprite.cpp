#include "LiteRGSS.h"
#include "log.h"

#include "CGraphics.h"

#include "Common/Rectangle.h"

#include "RectangleElement.h"
#include "TextureElement.h"
#include "SpriteElement.h"
#include "ViewportElement.h"

VALUE rb_cSprite = Qnil;

template<>
void rb::Mark<SpriteElement>(SpriteElement* spritePtr) {
	if (spritePtr == nullptr) {
		return;
	}
	auto& sprite = *spritePtr;
	rb_gc_mark(sprite.rViewport);
	rb_gc_mark(sprite.rBitmap);
	rb_gc_mark(sprite.rX);
	rb_gc_mark(sprite.rY);
	rb_gc_mark(sprite.rZ);
	rb_gc_mark(sprite.rOX);
	rb_gc_mark(sprite.rOY);
	rb_gc_mark(sprite.rAngle);
	rb_gc_mark(sprite.rZoomX);
	rb_gc_mark(sprite.rZoomY);
	rb_gc_mark(sprite.rRect);
}

VALUE rb_Sprite_Initialize(int argc, VALUE* argv, VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);

	// If a viewport was specified 
	if(argc == 1 && rb_obj_is_kind_of(argv[0], rb_cViewport) == Qtrue) {
		auto& viewport = rb::Get<ViewportElement>(argv[0]);		
		if (viewport.instance() == nullptr) {
			rb_raise(rb_eRGSSError, "Invalid viewport provided to instanciate a Sprite.");
			return Qnil;
		}
		sprite.init(cgss::Sprite::create(*viewport.instance()));
		sprite.rViewport = argv[0];
	}
	/*
	// If a window is specified 
	else if (argc == 1 && rb_obj_is_kind_of(argv[0], rb_cWindow) == Qtrue)
	{
		//TODO
		auto& window = rb::GetSafe<CWindow_Element>(argv[0], rb_cWindow);
		window.add(sprite);
		sprite.rViewport = argv[0];
		VALUE opacity = LONG2NUM(NUM2LONG(window.rOpacity) * NUM2LONG(window.rBackOpacity) / 255);
		rb_Sprite_setOpacity(self, opacity);
	}
	*/
	// Otherwise
	else {
		sprite.init(CGraphics::Get().add<cgss::Sprite>());
	}	

	/* Initializing Instance variables */
	return self;
}

VALUE rb_Sprite_Copy(VALUE self) {
	rb_raise(rb_eRGSSError, "Sprites cannot be cloned or duplicated.");
	return self;
}

VALUE rb_Sprite_Dispose(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	sprite->dispose();
	sprite.rViewport = Qnil;
	sprite.rRect = Qnil;
	sprite.rBitmap = Qnil;
	LOG("[Sprite] Disposed");
	return Qnil;
}

VALUE rb_Sprite_setBitmap(VALUE self, VALUE bitmap) {
	auto& sprite = rb::Get<SpriteElement>(self);
	
	if (bitmap == Qnil) {
		sprite->setVisible(false);
		sprite.rBitmap = bitmap;
		return self;
	}
	auto& bmp = rb::GetSafe<TextureElement>(bitmap, rb_cBitmap);

	sprite->setTexture(*bmp.instance(), true);
	sprite->setVisible(true);
	sprite.rBitmap = bitmap;

	if (!NIL_P(sprite.rRect)) {
		auto& rect = rb::Get<RectangleElement>(sprite.rRect);
		rect->setRect(sprite->getTextureRect());
	}
	return self;
}

VALUE rb_Sprite_getBitmap(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	return sprite.rBitmap;
}

VALUE rb_Sprite_setX(VALUE self, VALUE val) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto newX = static_cast<float>(rb_num2long(val));
	sprite->move(newX, sprite->getY());
	sprite.rX = val;
	return val;
}

VALUE rb_Sprite_getX(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	return sprite.rX;
}

VALUE rb_Sprite_setY(VALUE self, VALUE val) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto newY = static_cast<float>(rb_num2long(val));
	sprite->move(sprite->getX(), newY);
	sprite.rY = val;
	return val;
}

VALUE rb_Sprite_getY(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	return sprite.rY;
}

VALUE rb_Sprite_setZ(VALUE self, VALUE val) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto newZ = rb_num2long(val);
	sprite->setZ(newZ);
	sprite.rZ = val;
	return val;
}

VALUE rb_Sprite_getZ(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	return sprite.rZ;
}

VALUE rb_Sprite_setOX(VALUE self, VALUE val) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto newOx = static_cast<float>(rb_num2long(val));
	sprite->moveOrigin(newOx, sprite->getOy());
	sprite.rOX = val;
	return val;
}

VALUE rb_Sprite_getOX(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	return sprite.rOX;
}

VALUE rb_Sprite_setOY(VALUE self, VALUE val) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto newOy = static_cast<float>(rb_num2long(val));
	sprite->moveOrigin(sprite->getOx(), newOy);
	sprite.rOY = val;
	return val;
}

VALUE rb_Sprite_getOY(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	return sprite.rOY;
}

VALUE rb_Sprite_setVisible(VALUE self, VALUE val) {
	auto& sprite = rb::Get<SpriteElement>(self);
	sprite->setVisible(RTEST(val));
	return val;
}

VALUE rb_Sprite_getVisible(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	return sprite->isVisible() ? Qtrue : Qfalse;
}

VALUE rb_Sprite_setAngle(VALUE self, VALUE val) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto newAngle = static_cast<float>(NUM2DBL(val));
	sprite->setAngle(newAngle);
	sprite.rAngle = val;
	return val;
}

VALUE rb_Sprite_getAngle(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	return sprite.rAngle;
}

VALUE rb_Sprite_setZoomX(VALUE self, VALUE val) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto newZoomX = static_cast<float>(rb_num2dbl(val));
	sprite->scale(newZoomX, sprite->getScaleY());
	sprite.rZoomX = val;
	return val;
}

VALUE rb_Sprite_getZoomX(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	return sprite.rZoomX;
}

VALUE rb_Sprite_setZoomY(VALUE self, VALUE val) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto newZoomY = static_cast<float>(rb_num2dbl(val));
	sprite->scale(sprite->getScaleX(), newZoomY);
	sprite.rZoomY = val;
	return val;
}

VALUE rb_Sprite_getZoomY(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	return sprite.rZoomY;
}

VALUE rb_Sprite_setPosition(VALUE self, VALUE x, VALUE y) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto newX = static_cast<float>(rb_num2long(x));
	const auto newY = static_cast<float>(rb_num2long(y));
	sprite->move(newX, newY);
	sprite.rX = x;
	sprite.rY = y;
	return self;
}

VALUE rb_Sprite_setOrigin(VALUE self, VALUE x, VALUE y) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto newOx = static_cast<float>(rb_num2long(x));
	const auto newOy = static_cast<float>(rb_num2long(y));
	sprite->moveOrigin(newOx, newOy);
	sprite.rOX = x;
	sprite.rOY = y;
	return self;
}

VALUE rb_Sprite_setZoom(VALUE self, VALUE zoom) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto scale = static_cast<float>(rb_num2dbl(zoom));
	sprite->scale(scale, scale);
	sprite.rZoomX = zoom;
	sprite.rZoomY = zoom;
	return zoom;
}

VALUE rb_Sprite_setOpacity(VALUE self, VALUE val) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto newOpacity = normalize_long(rb_num2long(val), 0, 255);
	const auto& oldColor = sprite->getColor();
	sprite->setColor(sf::Color(oldColor.r, oldColor.g, oldColor.b, newOpacity));
	return val;
}

VALUE rb_Sprite_getOpacity(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto& color = sprite->getColor();
	return rb_int2inum(color.a);
}

VALUE rb_Sprite_getRect(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	VALUE rc = sprite.rRect;

	// Either it's already initialized, so we just have to return the VALUE
	if (!NIL_P(rc)) {
		return rc;
	}

	// Or we need to perform a lazy initialization of rRect
	/* Creating rect */
	VALUE argv[2];
	argv[0] = argv[1] = LONG2FIX(0);
	rc = rb_class_new_instance(2, argv, rb_cRect);

	auto& rect = rb::Get<RectangleElement>(rc);
	/* Setting rect parameter */
	rect->setRect(sprite->getTextureRect());

	/* Linking Rect */
	sprite->bindRectangle(rect.instance());
	sprite.rRect = rc;
	return rc;
}

VALUE rb_Sprite_setRect(VALUE self, VALUE val) {
	VALUE rc = rb_Sprite_getRect(self);

	auto* rect1 = rb::GetSafeOrNull<RectangleElement>(val, rb_cRect);
	auto& rect2 = rb::Get<RectangleElement>(rc);

	auto rectangle = rect1 == nullptr || *rect1 == nullptr ? cgss::Rectangle{} : (*rect1->instance());
	rect2->setRect(rectangle.getRect());

	return val;
}

VALUE rb_Sprite_getMirror(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	return sprite.rMirror;
}

VALUE rb_Sprite_setMirror(VALUE self, VALUE val) {
	auto& sprite = rb::Get<SpriteElement>(self);
	const auto mirror = RTEST(val);
	sprite->setMirror(mirror);
	sprite.rMirror = mirror ? Qtrue : Qfalse;
	return self;
}

VALUE rb_Sprite_Viewport(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	return sprite.rViewport;
}

VALUE rb_Sprite_Index(VALUE self) {
	auto& sprite = rb::Get<SpriteElement>(self);
	return rb_uint2inum(sprite->getZ().index);
}

VALUE rb_Sprite_width(VALUE self) {
	VALUE rc = rb_Sprite_getRect(self);
	return rb_Rect_getWidth(rc);
}

VALUE rb_Sprite_height(VALUE self) {
	VALUE rc = rb_Sprite_getRect(self);
	return rb_Rect_getHeight(rc);
}

void Init_Sprite() {
	rb_cSprite = rb_define_class_under(rb_mLiteRGSS, "Sprite", rb_cDrawable);
	rb_define_alloc_func(rb_cSprite, rb::Alloc<SpriteElement>);

	rb_define_method(rb_cSprite, "initialize", _rbf rb_Sprite_Initialize, -1);
	rb_define_method(rb_cSprite, "dispose", _rbf rb_Sprite_Dispose, 0);
	rb_define_method(rb_cSprite, "bitmap", _rbf rb_Sprite_getBitmap, 0);
	rb_define_method(rb_cSprite, "bitmap=", _rbf rb_Sprite_setBitmap, 1);
	rb_define_method(rb_cSprite, "x", _rbf rb_Sprite_getX, 0);
	rb_define_method(rb_cSprite, "x=", _rbf rb_Sprite_setX, 1);
	rb_define_method(rb_cSprite, "y", _rbf rb_Sprite_getY, 0);
	rb_define_method(rb_cSprite, "y=", _rbf rb_Sprite_setY, 1);
	rb_define_method(rb_cSprite, "z", _rbf rb_Sprite_getZ, 0);
	rb_define_method(rb_cSprite, "z=", _rbf rb_Sprite_setZ, 1);
	rb_define_method(rb_cSprite, "ox", _rbf rb_Sprite_getOX, 0);
	rb_define_method(rb_cSprite, "ox=", _rbf rb_Sprite_setOX, 1);
	rb_define_method(rb_cSprite, "oy", _rbf rb_Sprite_getOY, 0);
	rb_define_method(rb_cSprite, "oy=", _rbf rb_Sprite_setOY, 1);
	rb_define_method(rb_cSprite, "visible", _rbf rb_Sprite_getVisible, 0);
	rb_define_method(rb_cSprite, "visible=", _rbf rb_Sprite_setVisible, 1);
	rb_define_method(rb_cSprite, "angle", _rbf rb_Sprite_getAngle, 0);
	rb_define_method(rb_cSprite, "angle=", _rbf rb_Sprite_setAngle, 1);
	rb_define_method(rb_cSprite, "zoom_x", _rbf rb_Sprite_getZoomX, 0);
	rb_define_method(rb_cSprite, "zoom_x=", _rbf rb_Sprite_setZoomX, 1);
	rb_define_method(rb_cSprite, "zoom_y", _rbf rb_Sprite_getZoomY, 0);
	rb_define_method(rb_cSprite, "zoom_y=", _rbf rb_Sprite_setZoomY, 1);
	rb_define_method(rb_cSprite, "set_position", _rbf rb_Sprite_setPosition, 2);
	rb_define_method(rb_cSprite, "set_origin", _rbf rb_Sprite_setOrigin, 2);
	rb_define_method(rb_cSprite, "zoom=", _rbf rb_Sprite_setZoom, 1);
	rb_define_method(rb_cSprite, "opacity", _rbf rb_Sprite_getOpacity, 0);
	rb_define_method(rb_cSprite, "opacity=", _rbf rb_Sprite_setOpacity, 1);
	rb_define_method(rb_cSprite, "src_rect", _rbf rb_Sprite_getRect, 0);
	rb_define_method(rb_cSprite, "src_rect=", _rbf rb_Sprite_setRect, 1);
	rb_define_method(rb_cSprite, "viewport", _rbf rb_Sprite_Viewport, 0);
	rb_define_method(rb_cSprite, "mirror", _rbf rb_Sprite_getMirror, 0);
	rb_define_method(rb_cSprite, "mirror=", _rbf rb_Sprite_setMirror, 1);
	rb_define_method(rb_cSprite, "width", _rbf rb_Sprite_width, 0);
	rb_define_method(rb_cSprite, "height", _rbf rb_Sprite_height, 0);
	rb_define_method(rb_cSprite, "__index__", _rbf rb_Sprite_Index, 0);

	rb_define_method(rb_cSprite, "clone", _rbf rb_Sprite_Copy, 0);
	rb_define_method(rb_cSprite, "dup", _rbf rb_Sprite_Copy, 0);
}
