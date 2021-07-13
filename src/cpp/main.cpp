#include <emscripten.h>
#include <emscripten/bind.h>
#include <string>

std::string math_expression = "";

void add_number(std::string n) {
  math_expression += n;
};

std::string get_math_expression() {
  return math_expression;
};

EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("add_number", &add_number);
  emscripten::function("get_math_expression", &get_math_expression);
};
