#ifndef SMARTCALC_ADDITIONAL_FUNCS_H_
#define SMARTCALC_ADDITIONAL_FUNCS_H_

#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <string>

enum last_added { OUTPUT, STACK, NONE };

inline std::map<std::string, int> operations_map = {
    {"cos", 4},  {"sin", 4},  {"tan", 4}, {"acos", 4}, {"asin", 4},
    {"atan", 4}, {"sqrt", 4}, {"ln", 4},  {"log", 4},  {"^", 3},
    {"*", 2},    {"/", 2},    {"mod", 2}, {"+", 1},    {"-", 1}};

/**
 * Gets a substr from input string and checks
 * if it`s a function from operations_map or not.
 * Returns substr if true, empty string otherwise.
 */
template <class Container>
inline std::string getFunction(std::string::iterator &start_iterator) {
  auto end_iterator = start_iterator;
  while (isalpha(*end_iterator)) {
    ++end_iterator;
  }
  std::string function = std::string(start_iterator, end_iterator);
  start_iterator += function.length() - 1;
  if (operations_map.count(function)) {
    return function;
  } else {
    return std::string();
  }
}

template <class Container>
inline void handleCalculationErrors(Container output_polish) {
  if (output_polish.empty()) {
    throw std::invalid_argument("Incorrect expression");
  }
  std::string element = output_polish.front();
  if (!((element[0] == '-' || element[0] == '+') && element.length() > 1) &&
      !isdigit(element[0])) {
    throw std::invalid_argument("Not enough operands error");
  }
}

/**
 * Decides if an operations is an unar minus or unar plus
 * and pushes it to the output if it is.
 * Returns true if an operation is unar.
 */
template <class Container>
inline bool isUnar(std::string::iterator operation, Container &output,
                   last_added &last) {
  bool is_unar = false;
  if (*operation == '+' || *operation == '-') {
    if (output.empty() || (last == STACK && isdigit(*(operation + 1)))) {
      output.push_back(std::string(1, *operation));
      is_unar = true;
      last = OUTPUT;
    }
  }
  return is_unar;
}

inline void stringToLower(std::string &input_string) {
  std::transform(input_string.begin(), input_string.end(), input_string.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

/**
 * Adds digit to the output: if it refers to already
 * added num - concatenates digits, otherwise - just
 * pushes digit to the container.
 */
template <class Container>
inline void addNumToOutput(char val, Container &output_polish,
                           last_added &last) {
  if (!output_polish.empty() && last != STACK) {
    std::string prev_val = output_polish.back();
    prev_val.push_back(val);
    output_polish.pop_back();
    output_polish.push_back(prev_val);
  } else {
    output_polish.push_back(std::string(1, val));
  }
  last = OUTPUT;
}

/**
 * Adds dot to the number that has been
 * already added to the output container.
 */
template <class Container>
inline void addPointToOutput(Container &output_polish) {
  std::string prev_val = output_polish.back();
  prev_val.push_back('.');
  output_polish.pop_back();
  output_polish.push_back(prev_val);
}

/**
 * Checks if letter is a part of a function from operations_map.
 */
template <class Container>
inline bool isNotFunction(std::string &operation, std::string::iterator &c) {
  bool result = true;
  if (isalpha(*c)) {
    operation = getFunction<Container>(c);
    if (operation[0] != '\0') result = false;
  }
  return result;
}

/**
 * Checks if it is available to add the dot to the number
 * (if it already has a dot or not).
 */
template <class Container>
inline bool processDot(bool &decimal_num, last_added &last,
                       Container &output_polish, bool &parse_result) {
  bool result = false;
  if (decimal_num || last != OUTPUT) {
    parse_result = false;
  } else {
    addPointToOutput<Container>(output_polish);
    last = OUTPUT;
    decimal_num = true;
    result = true;
  }
  return result;
}

/**
 * Checks if the operation is a parenthesis and processes it
 * according to whether it is an opening or closing one.
 * Returns true if an input operation was a parenthesis, false
 * if not.
 */
template <class Container>
inline bool processBrackets(std::string operation,
                            std::stack<std::string> &operations_stack,
                            Container &output_polish) {
  bool is_bracket = false;
  if (operation == "(") {
    is_bracket = true;
    operations_stack.push(operation);
  } else if (operation == ")") {
    if (operations_stack.empty()) {
      throw std::invalid_argument(
          "Can not close parenthesis without operators");
    }

    is_bracket = true;
    while (operations_stack.top() != "(") {
      if (operations_stack.empty()) {
        throw std::invalid_argument("Unclosed parenthesis expression");
      }
      output_polish.push_back(operations_stack.top());
      operations_stack.pop();
    }
    operations_stack.pop();
  }
  return is_bracket;
}

/**
 * Checks if an input operation`s precedence is lower than the precedence
 * of an operation that is on the top of the operations_stack.
 */
inline bool currentPrecedenceLess(std::string operation,
                                  std::stack<std::string> &operations_stack) {
  return (operations_map[operation] < operations_map[operations_stack.top()]);
}

/**
 * Checks if an input operation`s precedence is higher than the precedence
 * of an operation that is on the top of the operations_stack.
 */
inline bool currentPrecedenceHigher(std::string operation,
                                    std::stack<std::string> &operations_stack) {
  return (operations_map[operation] > operations_map[operations_stack.top()]);
}

/**
 * Processes the operation according to it`s precedence and decides
 * what operations should pe pushed to output container and to the
 * operations stack.
 */
template <class Container>
inline void processPrecedence(std::string operation,
                              std::stack<std::string> &operations_stack,
                              Container &output_polish) {
  if (currentPrecedenceHigher(operation, operations_stack)) {
    operations_stack.push(operation);
  } else if (currentPrecedenceLess(operation, operations_stack)) {
    while (!operations_stack.empty() &&
           currentPrecedenceLess(operation, operations_stack)) {
      output_polish.push_back(operations_stack.top());
      operations_stack.pop();
    }
    operations_stack.push(operation);
  } else {
    if (operation == "^") {
      operations_stack.push(operation);
    } else {
      output_polish.push_back(operations_stack.top());
      operations_stack.pop();
      operations_stack.push(operation);
    }
  }
}

/**
 * Checks if current operation is incorrect by comparing
 * it to the previous lexem.
 */
inline void handleOperationsErrors(std::string curr, std::string prev) {
  bool is_error = false;
  if (!isdigit(prev[0])) {
    if ((curr != "(" && curr != ")") && !isalpha(curr[0]) && prev != ")") {
      is_error = true;
    }
  }
  if (is_error) throw std::invalid_argument("Incorrect number of operations");
}

/**
 * Checks if an operation is a parenthesis or not and calls additional
 * functions to push it to the operations stack.
 */
template <class Container>
inline void processOperation(std::string operation,
                             std::stack<std::string> &operations_stack,
                             Container &output_polish, last_added &last) {
  if (operation != "(" && operation != ")" && !operations_map.count(operation))
    throw std::invalid_argument("Undefined symbol.");
  if (operations_stack.empty()) {
    operations_stack.push(operation);
  } else {
    if (!processBrackets<Container>(operation, operations_stack,
                                    output_polish)) {
      processPrecedence<Container>(operation, operations_stack, output_polish);
    }
  }
  last = STACK;
}

/**
 * Pushes all remaining operations left in operations stack to
 * the output container after all.
 */
template <class Container>
inline void clearStack(std::stack<std::string> &operations_stack,
                       Container &output_polish) {
  while (!operations_stack.empty()) {
    output_polish.push_back(operations_stack.top());
    operations_stack.pop();
  }
}

/**
 * Calculates expression with one operand.
 */
inline double calculateFunction(std::stack<double> &calc_stack,
                                std::string operation) {
  double result = 0;
  if (operation == "cos")
    result = cos(calc_stack.top());
  else if (operation == "sin")
    result = sin(calc_stack.top());
  else if (operation == "tan")
    result = tan(calc_stack.top());
  else if (operation == "acos")
    result = acos(calc_stack.top());
  else if (operation == "asin")
    result = asin(calc_stack.top());
  else if (operation == "atan")
    result = atan(calc_stack.top());
  else if (operation == "sqrt")
    result = sqrt(calc_stack.top());
  else if (operation == "ln")
    result = log(calc_stack.top());
  else if (operation == "log") {
    if (calc_stack.top() == 0)
      throw std::invalid_argument("Log(0) is undefined number.");
    result = log10(calc_stack.top());
  }
  return result;
}

/**
 * Calculates expression with two operands.
 */
inline double calculateOperation(std::stack<double> &calc_stack,
                                 std::string operation) {
  if (calc_stack.size() < 2)
    throw std::invalid_argument("Incorrect number of operands.");
  double num2 = 0;
  num2 = calc_stack.top();
  calc_stack.pop();
  double result = 0;
  if (operation == "^") {
    result = pow(calc_stack.top(), num2);
  } else if (operation == "*") {
    result = num2 * calc_stack.top();
  } else if (operation == "/") {
    if (fabs(num2 - 0) < 1e-7) throw std::invalid_argument("Zero division");
    result = calc_stack.top() / num2;
  } else if (operation == "mod") {
    result = fmod(calc_stack.top(), num2);
  } else if (operation == "+") {
    result = num2 + calc_stack.top();
  } else if (operation == "-") {
    result = calc_stack.top() - num2;
  }
  return result;
}

/**
 * Calculates expression with two operands from calc_stack
 * and adds the intermediate result to output.
 */
template <class Container>
inline void calculateExpression(Container &output,
                                std::stack<double> &calc_stack,
                                std::string operation) {
  output.pop_front();
  if (operation.length() > 1 && operation != "mod") {
    output.push_front(std::to_string(calculateFunction(calc_stack, operation)));
  } else if (operation == "(" || operation == ")") {
    throw std::invalid_argument("Incorrect number of parenthesis");
  } else {
    output.push_front(
        std::to_string(calculateOperation(calc_stack, operation)));
  }
  calc_stack.pop();
}

/**
 * Parses input std::string into postfix polish notation
 * using users type of class to store the result of parsing
 * (should be implemented push_back and pop_back functions).
 * Returns container with parsed lexems if successful,
 * empty - if error occured.
 */
template <class Container = std::list<std::string>>
inline Container parseToPolishNotation(std::string input_string) {
  Container output_polish;
  std::stack<std::string> operations_stack;
  bool parse_result = true, decimal_num = false;
  last_added last_added = NONE;
  std::string prev = std::string(1, input_string[0]);
  stringToLower(input_string);
  for (auto c = input_string.begin(); c != input_string.end(); c++) {
    if (*c != '.' && !isdigit(*c)) {
      decimal_num = false;
    }
    if (isspace(*c)) {
      continue;
    } else if (isdigit(*c)) {
      addNumToOutput<Container>(*c, output_polish, last_added);
      prev = std::string(1, *c);
    } else if (*c == '.') {
      if (!processDot(decimal_num, last_added, output_polish, parse_result))
        break;
    } else {
      std::string operation(1, *c);
      if (isalpha(*c) && isNotFunction<Container>(operation, c)) {
        throw std::invalid_argument("Incorrect expression.");
      } else if (!isUnar<Container>(c, output_polish, last_added)) {
        processOperation<Container>(operation, operations_stack, output_polish,
                                    last_added);
        handleOperationsErrors(operation, prev);
      }
      prev = operation;
    }
  }
  clearStack<Container>(operations_stack, output_polish);

  return parse_result ? output_polish : Container();
}

#endif  // SMARTCALC_ADDITIONAL_FUNCS_H_
