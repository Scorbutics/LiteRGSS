#include <SFML/Graphics/Color.hpp>
#include "LiteRGSS.h"
#include "rbAdapter.h"
#include "NormalizeNumbers.h"
#include "Color.h"

VALUE rb_cColor = Qnil;

template<>
void rb::Mark<ColorElement>(ColorElement* color) {}

VALUE rb_Color_Initialize(int argc, VALUE* argv, VALUE self) {
	VALUE red, green, blue, alpha;
	rb_scan_args(argc, argv, "13", &red, &green, &blue, &alpha);
	auto& colorElement = rb::Get<ColorElement>(self);
	auto color = colorElement.getValue();
	if(RTEST(red))
		color.r = normalize_long(rb_num2long(red), 0, 255);
	if(RTEST(green))
		color.g = normalize_long(rb_num2long(green), 0, 255);
	if(RTEST(blue))
		color.b = normalize_long(rb_num2long(blue), 0, 255);
	if(RTEST(alpha))
		color.a = normalize_long(rb_num2long(alpha), 0, 255);
	colorElement.setValue(std::move(color));
	return self;
}

VALUE rb_Color_InitializeCopy(VALUE self, VALUE original) {
	auto& colorElement = rb::Get<ColorElement>(self);
	auto colorSource = rb::GetSafe<ColorElement>(original, rb_cColor).getValue();
	colorElement.setValue(std::move(colorSource));
	return self;
}

VALUE rb_Color_getRed(VALUE self) {
	auto& color = rb::Get<ColorElement>(self).getValue();
	return rb_int2inum(color.r);
}

VALUE rb_Color_setRed(VALUE self, VALUE red) {
	auto& color = rb::Get<ColorElement>(self);
	color.setR(normalize_long(rb_num2long(red), 0, 255));
	return self;
}

VALUE rb_Color_getGreen(VALUE self) {
	auto& color = rb::Get<ColorElement>(self).getValue();
	return rb_int2inum(color.g);
}

VALUE rb_Color_setGreen(VALUE self, VALUE red) {
	auto& color = rb::Get<ColorElement>(self);
	color.setG(normalize_long(rb_num2long(red), 0, 255));
	return self;
}

VALUE rb_Color_getBlue(VALUE self) {
	auto& color = rb::Get<ColorElement>(self).getValue();
	return rb_int2inum(color.b);
}

VALUE rb_Color_setBlue(VALUE self, VALUE red) {
	auto& color = rb::Get<ColorElement>(self);
	color.setB(normalize_long(rb_num2long(red), 0, 255));
	return self;
}

VALUE rb_Color_getAlpha(VALUE self) {
	auto& color = rb::Get<ColorElement>(self).getValue();
	return rb_int2inum(color.a);
}

VALUE rb_Color_setAlpha(VALUE self, VALUE red) {
	auto& color = rb::Get<ColorElement>(self);
	color.setA(normalize_long(rb_num2long(red), 0, 255));
	return self;
}

VALUE rb_Color_eql(VALUE self, VALUE other) {
	auto& color = rb::Get<ColorElement>(self).getValue();
	auto* color2 = rb::GetSafeOrNull<ColorElement>(other, rb_cColor);
	if (color2 == nullptr) {
		return Qfalse;
	}

	return color == color2->getValue();
}

VALUE rb_Color_Load(VALUE self, VALUE str) {
	rb_check_type(str, T_STRING);
	VALUE arr[4];
	if(RSTRING_LEN(str) < (sizeof(double) * 4)) {
		arr[2] = arr[1] = arr[0] = LONG2FIX(1);
		return rb_class_new_instance(3, arr, self);
	}
	double* color_data = reinterpret_cast<double*>(RSTRING_PTR(str));
	arr[0] = rb_int2inum(static_cast<long>(color_data[0]));
	arr[1] = rb_int2inum(static_cast<long>(color_data[1]));
	arr[2] = rb_int2inum(static_cast<long>(color_data[2]));
	arr[3] = rb_int2inum(static_cast<long>(color_data[3]));
	return rb_class_new_instance(4, arr, self);
}

VALUE rb_Color_Save(VALUE self, VALUE limit) {
	auto& color = rb::Get<ColorElement>(self).getValue();
	double color_data[4];
	color_data[0] = static_cast<double>(color.r);
	color_data[1] = static_cast<double>(color.g);
	color_data[2] = static_cast<double>(color.b);
	color_data[3] = static_cast<double>(color.a);
	return rb_str_new(reinterpret_cast<const char*>(color_data), sizeof(double) * 4);
}

VALUE rb_Color_to_s(VALUE self) {
	auto& color = rb::Get<ColorElement>(self).getValue();
	return rb_sprintf("(%d, %d, %d, %d)", color.r, color.g, color.b, color.a);
}

void Init_Color() {
	rb_cColor = rb_define_class_under(rb_mLiteRGSS, "Color", rb_cObject);
	rb_define_alloc_func(rb_cColor, rb::Alloc<ColorElement>);

	rb_define_method(rb_cColor, "initialize", _rbf rb_Color_Initialize, -1);
	rb_define_method(rb_cColor, "set", _rbf rb_Color_Initialize, -1);
	rb_define_method(rb_cColor, "initialize_copy", _rbf rb_Color_InitializeCopy, 1);
	rb_define_method(rb_cColor, "red", _rbf rb_Color_getRed, 0);
	rb_define_method(rb_cColor, "red=", _rbf rb_Color_setRed, 1);
	rb_define_method(rb_cColor, "green", _rbf rb_Color_getGreen, 0);
	rb_define_method(rb_cColor, "green=", _rbf rb_Color_setGreen, 1);
	rb_define_method(rb_cColor, "blue", _rbf rb_Color_getBlue, 0);
	rb_define_method(rb_cColor, "blue=", _rbf rb_Color_setBlue, 1);
	rb_define_method(rb_cColor, "alpha", _rbf rb_Color_getAlpha, 0);
	rb_define_method(rb_cColor, "alpha=", _rbf rb_Color_setAlpha, 1);
	rb_define_method(rb_cColor, "==", _rbf rb_Color_eql, 1);
	rb_define_method(rb_cColor, "===", _rbf rb_Color_eql, 1);
	rb_define_method(rb_cColor, "eql?", _rbf rb_Color_eql, 1);
	rb_define_method(rb_cColor, "to_s", _rbf rb_Color_to_s, 0);
	rb_define_method(rb_cColor, "inspect", _rbf rb_Color_to_s, 0);
	rb_define_method(rb_cColor, "_dump", _rbf rb_Color_Save, 1);
	rb_define_singleton_method(rb_cColor, "_load", _rbf rb_Color_Load, 1);
}
