#ifndef SMARTCALC_GRAPH_CALCULATION_H_
#define SMARTCALC_GRAPH_CALCULATION_H_
#include "calc_model.h"

namespace s21 {
class GraphCalculation {
 public:
  GraphCalculation(int min, int max, std::string exp);
  ~GraphCalculation() {}
  int calculateGraphValues();
  std::pair<std::vector<double>, std::vector<double>> getVectors();

 private:
  int min_x_;
  int min_y_;
  int max_x_;
  int max_y_;
  std::string expression_;
  std::vector<double> x_;
  std::vector<double> y_;
  void replaceExpression(std::string &src, const std::string sub, char sym);
};
}  // namespace s21

#endif  // SMARTCALC_GRAPH_CALCULATION_H_
