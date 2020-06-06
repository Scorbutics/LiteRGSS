#include "LiteRGSS.h"
#include "rbAdapter.h"

#include "ShaderSprite.h"
#include "BlendMode.h"
#include "Shader.h"

VALUE rb_cShaderSprite = Qnil;

template<>
void rb::Mark<ShaderSpriteElement>(ShaderSpriteElement* sprite) {
	if (sprite == nullptr) {
		return;
	}
	rb_gc_mark(sprite->rViewport);
	rb_gc_mark(sprite->rBitmap);
	rb_gc_mark(sprite->rX);
	rb_gc_mark(sprite->rY);
	rb_gc_mark(sprite->rZ);
	rb_gc_mark(sprite->rOX);
	rb_gc_mark(sprite->rOY);
	rb_gc_mark(sprite->rAngle);
	rb_gc_mark(sprite->rZoomX);
	rb_gc_mark(sprite->rZoomY);
	rb_gc_mark(sprite->rRect);
	rb_gc_mark(sprite->rRenderStates);
}

VALUE rb_ShaderSprite_getShader(VALUE self) {
	auto& sprite = rb::Get<ShaderSpriteElement>(self);
	return sprite.rRenderStates;
}

VALUE rb_ShaderSprite_setShader(VALUE self, VALUE shader) {
	auto& sprite = rb::Get<ShaderSpriteElement>(self);
	if (rb_obj_is_kind_of(shader, rb_cBlendMode) == Qtrue)  {
		auto* renderStates = rb::GetPtr<RenderStatesElement>(shader);
		if (renderStates != nullptr) {
			sprite.rRenderStates = shader;
			sprite->bindRenderStates(renderStates);
			return self;
		}
	}
	sprite.rRenderStates = Qnil;
	sprite->bindRenderStates(nullptr);
	return self;
}

void Init_ShaderSprite() {
	rb_cShaderSprite = rb_define_class_under(rb_mLiteRGSS, "ShaderedSprite", rb_cSprite);
	rb_define_alloc_func(rb_cShaderSprite, rb::Alloc<ShaderSpriteElement>);

	rb_define_method(rb_cShaderSprite, "shader", _rbf rb_ShaderSprite_getShader, 0);
	rb_define_method(rb_cShaderSprite, "shader=", _rbf rb_ShaderSprite_setShader, 1);

	rb_define_method(rb_cShaderSprite, "blendmode", _rbf rb_ShaderSprite_getShader, 0);
	rb_define_method(rb_cShaderSprite, "blendmode=", _rbf rb_ShaderSprite_setShader, 1);
}
