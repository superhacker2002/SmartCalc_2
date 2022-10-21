#ifndef SMARTCALC_CALC_CONTROLLER_H_
#define SMARTCALC_CALC_CONTROLLER_H_

#include "../logic/calc_model.h"
#include "../logic/graph_calculation.h"
using container = std::list<std::string>;

/**
 * Intermediary between view(gui) and model(buisness logic).
 * Class allows to get acces to functions of parsing
 * into polish notation expression and its calcululation.
 */
namespace s21 {
class CalcController {
 public:
  CalcController(std::string input_string);
  double getResult();

 private:
  s21::Calculation<container> calculation_;
};

/**
 * Intermediary between view(gui) and model(buisness logic).
 * Class allows to get acces to functions calculation
 * of needed values for printing graph on user interface.
 */
class GraphController {
 public:
  GraphController(int min, int max, std::string exp);
  int makeGraph();
  std::pair<std::vector<double>, std::vector<double>> getVectors();

 private:
  s21::GraphCalculation graph_;
};
}  // namespace s21

#endif  // SMARTCALC_CALC_CONTROLLER_H_
