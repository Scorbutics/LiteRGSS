#ifndef Texture_Bitmap_H
#define Texture_Bitmap_H

#include <LiteCGSS/Graphics/Texture.h>
#include "CgssWrapper.h"
#include "RubyValue.h"

using TextureElement = CgssInstance<cgss::Texture>;

extern VALUE rb_cBitmap;
void Init_Bitmap();

#endif