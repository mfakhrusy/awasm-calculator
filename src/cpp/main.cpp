#include <emscripten.h>
#include <emscripten/bind.h>
#include <string>

std::string math_expression = "";

void add_number(std::string n) {
  math_expression += n;
};

namespace OperatorType {
  enum Value {
    add = 1,
    subtract = 2,
    multiply = 3,
    divide = 4,
    no_op = 5,
  };

  Value from_string_to_operator (std::string s_operator) {

    if (s_operator == std::string(1, '+')) {
      return add;
    } else if (s_operator == std::string(1, '-')) {
      return subtract;
    } else if (s_operator == std::string(1, 'x')) {
      return multiply;
    } else if (s_operator == std::string(1, '/')) {
      return divide;
    } else {
      return no_op;
    }
  }
};

void add_operator(std::string new_operator) {
  const std::string last_expression_string = std::string(1, math_expression[math_expression.length() - 1]);
  const OperatorType::Value last_expression = OperatorType::from_string_to_operator(last_expression_string);

  if (last_expression == OperatorType::no_op) {
    math_expression += new_operator;
  }
}

void clear_math_expression() {
  math_expression = "";
}

std::string get_math_expression() {
    return math_expression;
};

EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("get_math_expression", &get_math_expression);
  emscripten::function("add_number", &add_number);
  emscripten::function("add_operator", &add_operator);
  emscripten::function("clear_math_expression", &clear_math_expression);
};
