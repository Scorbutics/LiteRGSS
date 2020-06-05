#include <cassert>

#include "../LiteRGSS/LiteRGSS.h"

#include "ShaderFactory_test.h"

extern "C" {
  void Init_LiteRGSS();
}

extern "C" {
  void Init_LiteRGSS_test() {
    Init_LiteRGSS();
    Init_ShaderFactoryTest();
  }
}
