#ifndef SMARTCALC_CALC_MODEL_H_
#define SMARTCALC_CALC_MODEL_H_

#include <list>

#include "additional_funcs.h"

/**
 * Class with method to calculate infix notation
 * expression by converting it to reverse postfix
 * notaiton.
 * */
namespace s21 {
template <class Container = std::list<std::string>>
class Calculation {
 public:
  Calculation(std::string input_string)
      : output_polish_(parseToPolishNotation<Container>(input_string)) {}
  ~Calculation() {}

  double calcPolishNotationExp() {
    handleCalculationErrors(output_polish_);
    std::string element = output_polish_.front();
    while (1) {
      element = output_polish_.front();
      if (isdigit(element[0]) ||
          ((element[0] == '-' || element[0] == '+') && isdigit(element[1]))) {
        calc_stack_.push(std::stod(element));
        if (output_polish_.size() == 1) break;
        output_polish_.pop_front();
      } else {
        calculateExpression<Container>(output_polish_, calc_stack_, element);
      }
    }
    if (calc_stack_.size() > 1)
      throw std::invalid_argument("Incorrect expression");
    return std::stod(output_polish_.front());
  }

 private:
  Container output_polish_;
  std::stack<double> calc_stack_;
};
}  // namespace s21

#endif  // SMARTCALC_CALC_MODEL_H_
