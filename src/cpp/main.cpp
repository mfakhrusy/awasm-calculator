#include <emscripten.h>
#include <emscripten/bind.h>
#include <string>
#include <numeric>
#include <vector>

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

  Value from_string_to_operator (string s_operator) {
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
      case divide:
        return true;
      case add:
      case subtract:
        if (target == subtract || target == add) {
          return true;
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
  const OperatorType::Value last_expression = OperatorType::from_string_to_operator(last_expression_string);

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
    OperatorType::Value first_letter_op = OperatorType::from_string_to_operator(first_letter);

    if (first_letter_op == OperatorType::no_op) {
      // math_expr_rpn += first_letter;
      math_expr_rpn.push_back(first_letter);
    } else {
      if (tmp_op_rpn.empty()) {
        // tmp_op_rpn += first_letter;
        tmp_op_rpn.push_back(first_letter);
      } else {
        // OperatorType::Value target = OperatorType::from_string_to_operator(tmp_op_rpn.substr(tmp_op_rpn.length() - 1, 1));
        OperatorType::Value target = OperatorType::from_string_to_operator(tmp_op_rpn[tmp_op_rpn.size() - 1]);
        OperatorType::Value src = first_letter_op;

        if (OperatorType::is_operator_higher_precedence(src, target)) {
          // tmp_op_rpn += first_letter;
          tmp_op_rpn.push_back(first_letter);
        } else {
          // math_expr_rpn += tmp_op_rpn;
          math_expr_rpn.insert(math_expr_rpn.end(), tmp_op_rpn.begin(), tmp_op_rpn.end());
          // tmp_op_rpn = first_letter;
          tmp_op_rpn.clear();
          tmp_op_rpn.push_back(first_letter);
        }
      }
    }

    math_expression.erase(0, 1);
  }

  // math_expr_rpn += tmp_op_rpn;
  math_expr_rpn.insert(math_expr_rpn.end(), tmp_op_rpn.begin(), tmp_op_rpn.end());

  return math_expr_rpn;
}

double calc_rpn(string math_expr_rpn) {
  double result = 0;
  while (math_expr_rpn.length() > 1) {
    if (math_expr_rpn.length() == 3) {
      double left_operand = std::stod(math_expr_rpn.substr(0, 1));
      double right_operand = std::stod(math_expr_rpn.substr(1, 1));
      OperatorType::Value op = OperatorType::from_string_to_operator(math_expr_rpn.substr(2, 1));

      result += OperatorType::execute_operation(left_operand, right_operand, op);

      math_expr_rpn = "";
    } else {
      for (string::size_type i = 0; i < math_expr_rpn.length(); i++) {
        OperatorType::Value tmp_op = OperatorType::from_string_to_operator(math_expr_rpn.substr(i, 1));
        if (tmp_op != OperatorType::no_op) {
          double left_operand = std::stod(math_expr_rpn.substr(i - 2, 1));
          double right_operand = std::stod(math_expr_rpn.substr(i - 1, 1));

          double tmp_result = OperatorType::execute_operation(left_operand, right_operand, tmp_op);

          result += tmp_result;

          math_expr_rpn.replace(i, 1, std::to_string(tmp_result));
          math_expr_rpn.erase(i-2, 2);
        }
      }
    }
  }

  return result;
}

double get_math_result() {
  // implement shunting yarn
  vector<string> math_expr_rpn = get_rpn(math_expression);
string s = std::accumulate(std::begin(math_expr_rpn), std::end(math_expr_rpn), string(),
                                [](string &ss, string &s)
                                {
                                    return ss.empty() ? s : ss + "," + s;
                                });

  printf("math rpn %s | %lu\n", s.c_str(), math_expr_rpn.size());
  

  // // calculate math expression from reverse polish notation
  // double result = calc_rpn(math_expr_rpn);

  // math_expression = "";

  return -99.99;
}

EMSCRIPTEN_BINDINGS(my_module) {
  emscripten::function("get_math_expression", &get_math_expression);
  emscripten::function("add_number", &add_number);
  emscripten::function("add_operator", &add_operator);
  emscripten::function("remove_last_character", &remove_last_character);
  emscripten::function("clear_math_expression", &clear_math_expression);
  emscripten::function("get_math_result", &get_math_result);
};
