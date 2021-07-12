#include <emscripten.h>

extern "C" EMSCRIPTEN_KEEPALIVE int plusOne(int x) { return x + 1; };
