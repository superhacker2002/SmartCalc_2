#include "controller.h"

/**
 * Parametrized constructor for initializing private object
 * of Calculation class with the input string.
 */
s21::CalcController::CalcController(std::string input_string)
    : calculation_(input_string) {}

/**
 * Method calls Calculation class method for calculation
 * of the polish notation expression.
 */
double s21::CalcController::getResult() {
  return calculation_.calcPolishNotationExp();
}

/**
 * Parametrized constructor for initializing private object
 * of GraphCalculation class with the input parameters.
 */
s21::GraphController::GraphController(int min, int max, std::string exp)
    : graph_(min, max, exp) {}

/**
 * Method calls GraphCalculation class method for calculation of
 * data that is needed for printing a graph on gui.
 */
int s21::GraphController::makeGraph() { return graph_.calculateGraphValues(); }

/**
 * Accessor to vectors with coordinates that are needed for
 * printing a graph on gui.
 */
std::pair<std::vector<double>, std::vector<double>>
s21::GraphController::getVectors() {
  return graph_.getVectors();
}