#ifndef L_SPRITE_HEADER
#define L_SPRITE_HEADER
#include "Graphics.h"
#include "rbAdapter.h"



namespace rb {
	template<>
	void Mark<CSprite_Element>(CSprite_Element* sprite);
}
#endif