#include "graph_calculation.h"

s21::GraphCalculation::GraphCalculation(int min, int max, std::string exp)
    : min_x_(min), max_x_(max), expression_(exp) {}

void s21::GraphCalculation::replaceExpression(std::string &src,
                                              const std::string sub, char sym) {
  size_t pos = 0;
  while ((pos = src.find(sym)) != std::string::npos) src.replace(pos, 1, sub);
}

/**
 * Calculates the values of a function in a user-defined
 * definition area and writes all neede coordinates in
 * two vectors for x and y. Returns 0 if calculation
 * was successful.
 */
int s21::GraphCalculation::calculateGraphValues() {
  int result = 0;
  if (expression_.find('x') == std::string::npos) {
    result = 1;
  } else if (max_x_ == min_x_) {
    result = 2;
  } else {
    double step = 0.005 * (fabs(min_x_) + fabs(max_x_));
    double Y = 0;
    double X = (double)min_x_;
    min_x_ = 0;
    max_y_ = 0;
    std::string expression_tmp = expression_;
    while (X < (double)max_x_) {
      x_.push_back(X);
      replaceExpression(expression_tmp, std::to_string(X), 'x');
      Calculation calculation(expression_tmp);
      double result = calculation.calcPolishNotationExp();
      Y = result;
      if (X == (double)min_x_) {
        min_y_ = Y;
        max_y_ = Y;
        if (min_y_ == 0) {
          min_y_ = -1;
          max_y_ = 1;
        }
      }
      if (Y < min_y_) min_y_ = Y;
      if (Y > max_y_) max_y_ = Y;
      y_.push_back(Y);
      X += step;
      expression_tmp = expression_;
    }
  }
  return result;
}

std::pair<std::vector<double>, std::vector<double>>
s21::GraphCalculation::getVectors() {
  return std::make_pair(x_, y_);
}
