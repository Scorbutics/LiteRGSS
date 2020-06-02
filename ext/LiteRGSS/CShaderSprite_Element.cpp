#include "CShaderSprite_Element.h"


/*
void CShaderSprite_Element::draw(sf::RenderTarget& target) const
{
	if(drawable && visible)
	{
		if (render_states == nullptr)
			target.draw(sprite);
		else
			target.draw(sprite, *render_states);//, render_states->shader);
	}
}

void CShaderSprite_Element::drawFast(sf::RenderTarget& target) const
{
	draw(target);
}

*/

void CShaderSprite_Element::setRenderState(sf::RenderStates * states)
{
	render_states = states;
}
