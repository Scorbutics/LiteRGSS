#ifndef L_SPRITE_HEADER
#define L_SPRITE_HEADER
#include "Graphics.h"
#include "rbAdapter.h"

struct SpriteElement;
namespace rb {
	template<>
	void Mark<SpriteElement>(SpriteElement* sprite);
}
#endif