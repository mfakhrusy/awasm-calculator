#include <emscripten.h>
#include <emscripten/bind.h>
#include <string>
#include <cmath>

using std::string;
using std::vector;

string math_expression = "";

double math_result;

void add_number(string n) {
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

  Value from_string (string s_operator) {
    if (s_operator == string(1, '+')) {
      return add;
    } else if (s_operator == string(1, '-')) {
      return subtract;
    } else if (s_operator == string(1, 'x')) {
      return multiply;
    } else if (s_operator == string(1, '/')) {
      return divide;
    } else {
      return no_op;
    }
  }

  bool is_operator_higher_precedence (Value src, Value target) {
    switch (src) {
      case no_op:
        return false;
      case multiply:
        if (target == divide) {
          return false;
        } else {
          return true;
        }
      case divide:
        return true;
      case add:
        if (target == subtract) {
          return false;
        } else if (target == add) {
          return true;
        } else {
          return false;
        }
      case subtract:
        if (target == add) {
          return true;
        } else if (target == subtract) {
          return false;
        } else {
          return false;
        }
    }
  }

  double execute_operation (double left_operand, double right_operand, Value op) {
    switch (op) {
      case no_op:
        return 0;
      case multiply:
        return left_operand * right_operand;
      case divide:
        return left_operand / right_operand;
      case add:
        return left_operand + right_operand;
      case subtract:
        return left_operand - right_operand;
    }
  }
};

void add_operator(string new_operator) {
  const string last_expression_string = string(1, math_expression[math_expression.length() - 1]);
  const OperatorType::Value last_expression = OperatorType::from_string(last_expression_string);

  if (last_expression == OperatorType::no_op) {
    math_expression += new_operator;
  } else {
    math_expression.pop_back();
    math_expression += new_operator;
  }
}

void remove_last_character() {
  if (math_expression.length() > 0) {
    math_expression.pop_back();
  }
}

void clear_math_expression() {
  math_expression = "";
}

string get_math_expression() {
  return math_expression;
};

// rpn stands for reverse polish notation
vector<string> get_rpn(string math_expression) {
  vector<string> math_expr_rpn;
  vector<string> tmp_op_rpn;

  while (math_expression.length() > 0) {

    string first_letter = math_expression.substr(0, 1);
    OperatorType::Value first_letter_op = OperatorType::from_string(first_letter);

    string first_expr = first_letter;

    string::size_type next_expr_index = 1;

    if (first_letter_op == OperatorType::no_op) {
      for (string::size_type i = next_expr_index; i < math_expression.length(); ++i) {
        OperatorType::Value tmp = OperatorType::from_string(math_expression.substr(i, 1));

        if (tmp != OperatorType::no_op) {
          next_expr_index = i;
          first_expr = math_expression.substr(0, next_expr_index);
          break;
        }

        if (i == math_expression.length() - 1) {
          first_expr = math_expression;
          break;
        }
      }

      math_expr_rpn.push_back(first_expr);
    } else {
      if (tmp_op_rpn.empty()) {
        tmp_op_rpn.push_back(first_expr);
      } else {
        OperatorType::Value target = OperatorType::from_string(tmp_op_rpn[tmp_op_rpn.size() - 1]);
        OperatorType::Value src = OperatorType::from_string(first_expr);

        if (OperatorType::is_operator_higher_precedence(src, target)) {
          tmp_op_rpn.push_back(first_expr);
        } else {
          std::reverse(tmp_op_rpn.begin(), tmp_op_rpn.end());

          math_expr_rpn.insert(math_expr_rpn.end(), tmp_op_rpn.begin(), tmp_op_rpn.end());
          tmp_op_rpn.clear();
          tmp_op_rpn.push_back(first_expr);
        }
      }
    }

    if (first_expr == math_expression) {
      math_expression = "";
    } else {
      math_expression.erase(0, next_expr_index);
    }
  }

  std::reverse(tmp_op_rpn.begin(), tmp_op_rpn.end());

  math_expr_rpn.insert(math_expr_rpn.end(), tmp_op_rpn.begin(), tmp_op_rpn.end());

  return math_expr_rpn;
}

double calc_rpn(vector<string> math_expr_rpn) {
  double result = 0;

  while (math_expr_rpn.size() > 1) {
    if (math_expr_rpn.size() == 3) {
      double left_operand = std::stod(math_expr_rpn[0]);
      double right_operand = std::stod(math_expr_rpn[1]);
      OperatorType::Value op = OperatorType::from_string(math_expr_rpn[2]);

      result = OperatorType::execute_operation(left_operand, right_operand, op);

      math_expr_rpn.clear();
    } else {
      for (string::size_type i = 0; i < math_expr_rpn.size(); i++) {
        OperatorType::Value tmp_op = OperatorType::from_string(math_expr_rpn[i]);

        if (tmp_op != OperatorType::no_op) {
          double left_operand = std::stod(math_expr_rpn[i - 2]);
          double right_operand = std::stod(math_expr_rpn[i - 1]);

          double tmp_result = OperatorType::execute_operation(left_operand, right_operand, tmp_op);

          math_expr_rpn[i] = std::to_string(tmp_result);
          math_expr_rpn.erase(math_expr_rpn.begin() + i - 2, math_expr_rpn.begin() + i);
          break;
        }
      }
    }
  }

  return result;
}

double get_math_result() {
  // implement shunting yarn
  vector<string> math_expr_rpn = get_rpn(math_expression);

  // calculate math expression from reverse polish notation
  double result = std::ceil(calc_rpn(math_expr_rpn) * 1000.0) / 1000.0;


  return result;
}

EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("get_math_expression", &get_math_expression);
  emscripten::function("add_number", &add_number);
  emscripten::function("add_operator", &add_operator);
  emscripten::function("remove_last_character", &remove_last_character);
  emscripten::function("clear_math_expression", &clear_math_expression);
  emscripten::function("get_math_result", &get_math_result);
};
