#ifndef CShaderSprite_Element_H
#define CShaderSprite_Element_H

#include <SFML/Graphics/RenderStates.hpp>
#include "RubyValue.h"

class CShaderSprite_Element {
private:
	sf::RenderStates* render_states = nullptr;
public:
	CShaderSprite_Element() = default;
	~CShaderSprite_Element() = default;

	void setRenderState(sf::RenderStates* states);

	VALUE rRenderStates = Qnil;
};

#endif
