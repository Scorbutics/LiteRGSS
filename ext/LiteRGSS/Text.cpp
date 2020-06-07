#include "LiteRGSS.h"
#include "Text.h"
#include "NormalizeNumbers.h"
#include "rbAdapter.h"
#include "Fonts.h"

#include "FramedView_Window.h"
#include "Viewport.h"
#include "GraphicsSingleton.h"
#include "Drawable_Disposable.h"
#include "Color.h"

VALUE rb_cText = Qnil;

template<>
void rb::Mark<TextElement>(TextElement* text) {
	if (text == nullptr) {
		return;
	}
	rb_gc_mark(text->rViewport);
	rb_gc_mark(text->rwidth);
	rb_gc_mark(text->rheight);
	rb_gc_mark(text->rtext);
	rb_gc_mark(text->rAlign);
	rb_gc_mark(text->rX);
	rb_gc_mark(text->rY);
	rb_gc_mark(text->rZ);
}

VALUE rb_Text_getOpacity(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return rb_int2inum(text->getOpacity());
}

VALUE rb_Text_setOpacity(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	auto opacity = normalize_long(rb_num2long(val), 0, 255);
	text->setOpacity(static_cast<std::uint8_t>(opacity));
	return self;
}

VALUE rb_Text_Copy(VALUE self) {
	rb_raise(rb_eRGSSError, "Text can not be cloned or duplicated.");
	return self;
}

VALUE rb_Text_Dispose(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	text->dispose();
	return Qnil;
}

VALUE rb_Text_Disposed(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return text->isDisposed() ? Qtrue : Qfalse;
}

VALUE rb_Text_get_num_char(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	//TODO
	//return RB_UINT2NUM(text.getText().getNumCharToDraw());
	return RB_UINT2NUM(1);
}

VALUE rb_Text_set_draw_shadow(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	//TODO
	//text.getText().setDrawShadow(RTEST(val));
	return self;
}

VALUE rb_Text_get_draw_shadow(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	//TODO
	//return (text.getText().getDrawShadow() ? Qtrue : Qfalse);
	return Qfalse;
}

VALUE rb_Text_getRealWidth(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return rb_int2inum(text->getRealWidth());
}

VALUE rb_Text_get_fill_color(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	VALUE argv[4];
	sf::Color col = text->getFillColor();
	argv[0] = LONG2FIX(col.r);
	argv[1] = LONG2FIX(col.g);
	argv[2] = LONG2FIX(col.b);
	argv[3] = LONG2FIX(col.a);
	return rb_class_new_instance(4, argv, rb_cColor);
}

VALUE rb_Text_set_fill_color(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	auto& color = rb::GetSafe<sf::Color>(val, rb_cColor);
	text->setFillColor(color);
	return val;
}

VALUE rb_Text_get_outline_color(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	VALUE argv[4];
	sf::Color col = text->getOutlineColor();
	argv[0] = LONG2FIX(col.r);
	argv[1] = LONG2FIX(col.g);
	argv[2] = LONG2FIX(col.b);
	argv[3] = LONG2FIX(col.a);
	return rb_class_new_instance(4, argv, rb_cColor);
}

VALUE rb_Text_set_outline_color(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	auto& color = rb::GetSafe<sf::Color>(val, rb_cColor);
	text->setOutlineColor(color);
	return val;
}

VALUE rb_Text_get_outline_thickness(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return rb_float_new(static_cast<double>(text->getOutlineThickness()));
}

VALUE rb_Text_set_outline_thickness(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	text->setOutlineThickness(static_cast<float>(rb_num2dbl(val)));
	return val;
}

VALUE rb_Text_load_color(VALUE self, VALUE id) {
	auto& text = rb::Get<TextElement>(self);
	rb_Text_set_fill_color(self, rb_Fonts_get_fill_color(rb_mFonts, id));
	
	//TODO
	/*
	if(text.getText().getDrawShadow())
		rb_Text_set_outline_color(self, rb_Fonts_get_shadow_color(rb_mFonts, id));
	else
	*/
		rb_Text_set_outline_color(self, rb_Fonts_get_outline_color(rb_mFonts, id));
	return self;
}

VALUE rb_Text_getZ(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return text.rZ;
}

VALUE rb_Text_setZ(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	text->setZ(rb_num2long(val));
	text.rZ = val;
	return self;
}

VALUE rb_Text_Index(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return rb_uint2inum(text->getZ().index);
}

VALUE rb_Text_Viewport(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return text.rViewport;
}

VALUE rb_Text_getItalic(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return (text->getStyle() & sf::Text::Style::Italic) ? Qtrue : Qfalse;
}

VALUE rb_Text_setItalic(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	auto style = text->getStyle();
	if (RTEST(val)) {
		style |= sf::Text::Style::Italic;
	} else {
		style &= ~(sf::Text::Style::Italic);
	}
	text->setStyle(style);
	return self;
}

VALUE rb_Text_getBold(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return (text->getStyle() & sf::Text::Style::Bold) ? Qtrue : Qfalse;
}

VALUE rb_Text_setBold(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	auto style = text->getStyle();
	if (RTEST(val)) {
		style |= sf::Text::Style::Bold;
	} else {
		style &= ~(sf::Text::Style::Bold);
	}
	text->setStyle(style);
	return self;
}

VALUE rb_Text_get_align(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return text.rAlign;
}

VALUE rb_Text_set_align(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	long align = rb_num2long(val);
	text->setAlign(align);
	text.rAlign = val;
	return self;
}

VALUE rb_Text_setPosition(VALUE self, VALUE x, VALUE y) {
	auto& text = rb::Get<TextElement>(self);
	rb_check_type(x, T_FIXNUM);
	rb_check_type(y, T_FIXNUM);
	text->move(NUM2LONG(x), NUM2LONG(y));
	text.rX = x;
	text.rY = y;
	return self;
}

VALUE rb_Text_get_x(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return text.rX;
}

VALUE rb_Text_set_x(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	rb_check_type(val, T_FIXNUM);
	text->move(NUM2LONG(val), text->getY());
	text.rX = val;
	return val;
}

VALUE rb_Text_get_y(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return text.rY;
}

VALUE rb_Text_set_y(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	rb_check_type(val, T_FIXNUM);
	text->move(text->getX(), NUM2LONG(val));
	text.rY = val;
	return val;
}

VALUE rb_Text_get_width(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return text.rwidth;
}

VALUE rb_Text_set_width(VALUE self, VALUE val) {
	rb_check_type(val, T_FIXNUM);
	auto& text = rb::Get<TextElement>(self);
	text->resize(text->getWidth(), NUM2LONG(val));
	text.rwidth = val;
	return val;
}

VALUE rb_Text_get_height(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return text.rheight;
}

VALUE rb_Text_set_height(VALUE self, VALUE val) {
	rb_check_type(val, T_FIXNUM);
	auto& text = rb::Get<TextElement>(self);
	text->resize(NUM2LONG(val), text->getHeight());
	text.rheight = val;
	return val;
}

VALUE rb_Text_get_size(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return rb_int2inum(text->getCharacterSize());
}

VALUE rb_Text_set_size(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	text->setCharacterSize(normalize_long(rb_num2long(val), 1, 0xFFFF));
	return self;
}

VALUE rb_Text_set_Text(VALUE self, VALUE str) {
	auto& text = rb::Get<TextElement>(self);
	rb_check_type(str, T_STRING);
	text.rtext = str;
	//TODO
	//text.getText().setLineHeight(rb_num2dbl(text.rheight));
	std::string stru8(RSTRING_PTR(str));
	text->setString(sf::String::fromUtf8(stru8.begin(), stru8.end()));
	return str;
}

VALUE rb_Text_get_text_width(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	rb_check_type(val, T_STRING);
	std::string stru8(RSTRING_PTR(val));
	//TODO
	//sf::Uint32 width = text.getText().getTextWidth(sf::String::fromUtf8(stru8.begin(), stru8.end()));
	//return RB_UINT2NUM(width);
	return RB_UINT2NUM(1);
}

VALUE rb_Text_get_Text(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return text.rtext;
}

VALUE rb_Text_get_visible(VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	return text->isVisible() ? Qtrue : Qfalse;
}

VALUE rb_Text_set_visible(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	text->setVisible(RTEST(val));
	return self;
}

VALUE rb_Text_set_num_char(VALUE self, VALUE val) {
	auto& text = rb::Get<TextElement>(self);
	//TODO
	//text.getText().setNumCharToDraw(rb_num2ulong(val));
	//rb_Text_UpdateI(text);
	return self;
}

void rb_Text_Load_Font(TextElement &text, VALUE self, VALUE fontid, VALUE colorid, VALUE sizeid) {
	VALUE fcol;
	VALUE ocol;
	// Load the default parameter
	if(NIL_P(colorid))
		colorid = fontid;
	if(NIL_P(sizeid))
		sizeid = fontid;
	// Load the fill color
	fcol = rb_Fonts_get_fill_color(rb_mFonts, colorid);
	if(rb_obj_is_kind_of(fcol, rb_cColor) == Qtrue)
		rb_Text_set_fill_color(self, fcol);

	// Load the outline color
	if(text->getOutlineThickness() < 1.0f) // Loading the shadow color
		ocol = rb_Fonts_get_shadow_color(rb_mFonts, colorid);
	else
		ocol = rb_Fonts_get_outline_color(rb_mFonts, colorid);
	if(rb_obj_is_kind_of(ocol, rb_cColor) == Qtrue)
		rb_Text_set_outline_color(self, ocol);
	
	// Load the font
	text->setFont(rb_Fonts_get_font(rb_num2long(fontid)));
	
	// Load the size
	VALUE size = rb_Fonts_get_default_size(rb_mFonts, sizeid);
	if(!NIL_P(size)) {
		text->setCharacterSize(static_cast<unsigned int>(normalize_long(rb_num2long(size), 1, 0xFFFF)));
	}
}

VALUE rb_Text_Initialize(int argc, VALUE* argv, VALUE self) {
	auto& text = rb::Get<TextElement>(self);
	VALUE fontid, viewport, x, y, width, height, str, align, outlinesize, colorid, sizeid;
	VALUE opacity = LONG2NUM(255);
	rb_scan_args(argc, argv,"74", &fontid, &viewport, &x, &y, &width, &height, &str, &align, &outlinesize, &colorid, &sizeid);
	
	/* Viewport */
	if(rb_obj_is_kind_of(viewport, rb_cViewport) == Qtrue) {
		auto& viewportEl = rb::Get<ViewportElement>(viewport);		
		if (viewportEl.instance() == nullptr) {
			rb_raise(rb_eRGSSError, "Invalid viewport provided to instanciate a Text.");
			return Qnil;
		}
		viewportEl.initAndAdd(text);
		text.rViewport = argv[0];
	}
	/* If a window is specified */
	else if (rb_obj_is_kind_of(viewport, rb_cWindow) == Qtrue) {
		auto& window = rb::Get<FramedViewElement>(viewport);	
		window.initAndAdd(text);
		text.rViewport = viewport;
		opacity = LONG2NUM(NUM2LONG(window.rOpacity) * NUM2LONG(window.rContentOpacity) / 255);
	} else {
		text.init(GraphicsSingleton::Get().add<cgss::Text>());
		text.rViewport = Qnil;
	}

	/* Surface */
	rb_check_type(x, T_FIXNUM);
	text.rX = x;
	rb_check_type(y, T_FIXNUM);
	text.rY = y;
	text->move(x, y);

	rb_check_type(width, T_FIXNUM);
	text.rwidth = width;
	rb_check_type(height, T_FIXNUM);
	text.rheight = height;

	text->resize(width, height);

	/* Aligment */
	if(!NIL_P(align)) {
		long ralign = rb_num2long(align);
		if (ralign <= 2 && ralign >= 0) {
			text.rAlign = align;
		}
		text->setAlign(ralign);
	}
	/* Outline size */
	if(!NIL_P(outlinesize)) {
		text->setOutlineThickness(static_cast<float>(rb_num2dbl(outlinesize)));
	}
	/* Font */
	rb_Text_Load_Font(text, self, fontid, colorid, sizeid);
	/* Text */
	rb_Text_set_Text(self, str);
	rb_Text_setOpacity(self, opacity);
	return self;
}

void Init_Text() {
	rb_cText = rb_define_class_under(rb_mLiteRGSS, "Text", rb_cDrawable);

	rb_define_alloc_func(rb_cText, rb::Alloc<TextElement>);

	rb_define_method(rb_cText, "initialize", _rbf rb_Text_Initialize, -1);
	rb_define_method(rb_cText, "dispose", _rbf rb_Text_Dispose, 0);
	rb_define_method(rb_cText, "disposed?", _rbf rb_Text_Disposed, 0);
	rb_define_method(rb_cText, "set_position", _rbf rb_Text_setPosition, 2);
	rb_define_method(rb_cText, "x", _rbf rb_Text_get_x, 0);
	rb_define_method(rb_cText, "x=", _rbf rb_Text_set_x, 1);
	rb_define_method(rb_cText, "y", _rbf rb_Text_get_y, 0);
	rb_define_method(rb_cText, "y=", _rbf rb_Text_set_y, 1);
	rb_define_method(rb_cText, "width", _rbf rb_Text_get_width, 0);
	rb_define_method(rb_cText, "width=", _rbf rb_Text_set_width, 1);
	rb_define_method(rb_cText, "height", _rbf rb_Text_get_height, 0);
	rb_define_method(rb_cText, "height=", _rbf rb_Text_set_height, 1);
	rb_define_method(rb_cText, "fill_color", _rbf rb_Text_get_fill_color, 0);
	rb_define_method(rb_cText, "fill_color=", _rbf rb_Text_set_fill_color, 1);
	rb_define_method(rb_cText, "outline_color", _rbf rb_Text_get_outline_color, 0);
	rb_define_method(rb_cText, "outline_color=", _rbf rb_Text_set_outline_color, 1);
	rb_define_method(rb_cText, "outline_thickness", _rbf rb_Text_get_outline_thickness, 0);
	rb_define_method(rb_cText, "outline_thickness=", _rbf rb_Text_set_outline_thickness, 1);
	rb_define_method(rb_cText, "load_color", _rbf rb_Text_load_color, 1);
	rb_define_method(rb_cText, "align", _rbf rb_Text_get_align, 0);
	rb_define_method(rb_cText, "align=", _rbf rb_Text_set_align, 1);
	rb_define_method(rb_cText, "size", _rbf rb_Text_get_size, 0);
	rb_define_method(rb_cText, "size=", _rbf rb_Text_set_size, 1);
	rb_define_method(rb_cText, "set_size", _rbf rb_Text_set_size, 1);
	rb_define_method(rb_cText, "text", _rbf rb_Text_get_Text, 0);
	rb_define_method(rb_cText, "text=", _rbf rb_Text_set_Text, 1);
	rb_define_method(rb_cText, "visible", _rbf rb_Text_get_visible, 0);
	rb_define_method(rb_cText, "visible=", _rbf rb_Text_set_visible, 1);
	rb_define_method(rb_cText, "draw_shadow", _rbf rb_Text_get_draw_shadow, 0);
	rb_define_method(rb_cText, "draw_shadow=", _rbf rb_Text_set_draw_shadow, 1);
	rb_define_method(rb_cText, "nchar_draw", _rbf rb_Text_get_num_char, 0);
	rb_define_method(rb_cText, "nchar_draw=", _rbf rb_Text_set_num_char, 1);
	rb_define_method(rb_cText, "real_width", _rbf rb_Text_getRealWidth, 0);
	rb_define_method(rb_cText, "opacity", _rbf rb_Text_getOpacity, 0);
	rb_define_method(rb_cText, "opacity=", _rbf rb_Text_setOpacity, 1);
	rb_define_method(rb_cText, "text_width", _rbf rb_Text_get_text_width, 1);
	rb_define_method(rb_cText, "z", _rbf rb_Text_getZ, 0);
	rb_define_method(rb_cText, "z=", _rbf rb_Text_setZ, 1);
	rb_define_method(rb_cText, "__index__", _rbf rb_Text_Index, 0);
	rb_define_method(rb_cText, "viewport", _rbf rb_Text_Viewport, 0);
	rb_define_method(rb_cText, "italic", _rbf rb_Text_getItalic, 0);
	rb_define_method(rb_cText, "italic=", _rbf rb_Text_setItalic, 1);
	rb_define_method(rb_cText, "bold", _rbf rb_Text_getBold, 0);
	rb_define_method(rb_cText, "bold=", _rbf rb_Text_setBold, 1);

	rb_define_method(rb_cText, "clone", _rbf rb_Text_Copy, 0);
	rb_define_method(rb_cText, "dup", _rbf rb_Text_Copy, 0);
}
