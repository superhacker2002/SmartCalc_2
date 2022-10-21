#include <gtest/gtest.h>

#include "calc_model.h"
using container = std::list<std::string>;
constexpr double precision = 1e-7;

TEST(additional_funcs, get_function) {
  std::string test_string("sin+");
  auto iterator = test_string.begin();
  EXPECT_EQ("sin", getFunction<container>(iterator));

  test_string = "sinn";
  iterator = test_string.begin();
  EXPECT_EQ("", getFunction<container>(iterator));

  test_string = "";
  iterator = test_string.begin();
  EXPECT_EQ("", getFunction<container>(iterator));
}

TEST(additional_funcs, add_num_to_output) {
  std::list<std::string> output_polish = {"1"};
  last_added last = OUTPUT;
  addNumToOutput('3', output_polish, last);
  EXPECT_EQ("13", output_polish.back());

  last = STACK;
  addNumToOutput('3', output_polish, last);
  EXPECT_EQ("3", output_polish.back());
}

TEST(additional_funcs, add_point_to_output) {
  std::list<std::string> output_polish = {"1"};
  addPointToOutput(output_polish);
  EXPECT_EQ("1.", output_polish.back());
}

TEST(additional_funcs, process_dot) {
  std::list<std::string> output_polish;
  last_added last = NONE;
  bool parse_result = true;
  bool decimal_num = false;
  processDot(decimal_num, last, output_polish, parse_result);
  EXPECT_EQ(true, output_polish.empty());
  EXPECT_EQ(false, parse_result);

  output_polish = {"1"};
  last = OUTPUT;
  parse_result = true;
  decimal_num = false;
  processDot(decimal_num, last, output_polish, parse_result);
  EXPECT_EQ("1.", output_polish.back());
  EXPECT_EQ(true, decimal_num);

  output_polish = {"1."};
  last = OUTPUT;
  parse_result = true;
  decimal_num = true;
  processDot(decimal_num, last, output_polish, parse_result);
  EXPECT_EQ("1.", output_polish.back());
  EXPECT_EQ(false, parse_result);
}

TEST(additional_funcs, process_brackets) {
  std::list<std::string> output_polish = {"1"};
  std::string operation = "(";
  std::stack<std::string> operations_stack;
  operations_stack.push("+");
  EXPECT_EQ(true, processBrackets(operation, operations_stack, output_polish));
  EXPECT_EQ("(", operations_stack.top());

  output_polish.push_back("2");
  operations_stack.push("-");
  operation = ")";
  EXPECT_EQ(true, processBrackets(operation, operations_stack, output_polish));
  EXPECT_EQ("+", operations_stack.top());
  EXPECT_EQ("-", output_polish.back());
}

TEST(additional_funcs, process_precedence) {
  std::list<std::string> output_polish = {"1", "2"};
  std::string operation = "*";
  std::stack<std::string> operations_stack;
  operations_stack.push("+");
  processPrecedence(operation, operations_stack, output_polish);
  EXPECT_EQ("*", operations_stack.top());

  output_polish.push_back("3");
  operation = "-";
  processPrecedence(operation, operations_stack, output_polish);
  EXPECT_EQ("*", output_polish.back());
  EXPECT_EQ("-", operations_stack.top());
}

TEST(main_funcs, parenthesis_exceptions) {
  std::string input = "1/2+(2+3";
  s21::Calculation calculation(input);
  EXPECT_ANY_THROW(calculation.calcPolishNotaitonExp());

  input = ")";
  s21::Calculation calculation1(input);
  EXPECT_ANY_THROW(calculation1.calcPolishNotaitonExp());

  input = "(1+ 2))";
  s21::Calculation calculation2(input);
  EXPECT_ANY_THROW(calculation2.calcPolishNotaitonExp());

  input = "((1 + 2";
  s21::Calculation calculation3(input);
  EXPECT_ANY_THROW(calculation3.calcPolishNotaitonExp());
}

TEST(main_funcs, calculate_polish_exp) {
  std::string input = "(2+3)*5";
  s21::Calculation calculation(input);
  EXPECT_EQ(25.0f, calculation.calcPolishNotaitonExp());

  input = "1/2 + (2+3)/(sin(9-2)^2-6/7)";
  s21::Calculation calculation1(input);
  EXPECT_LE(precision, fabs(-11.2506 - calculation1.calcPolishNotaitonExp()));

  input =
      "sin(2) * cos(5) + tan(67^2) /\
            sin(128) - atan(sqrt(9)) - (4 mod 2) * log(5)";
  s21::Calculation calculation2(input);
  EXPECT_LE(precision, fabs(1.47532 - calculation2.calcPolishNotaitonExp()));

  input = "2^2^3";
  s21::Calculation calculation6(input);
  EXPECT_EQ(256.0, calculation6.calcPolishNotaitonExp());

  input = "2+-9";
  s21::Calculation calculation7(input);
  EXPECT_EQ(-7.0, calculation7.calcPolishNotaitonExp());

  input = "-9 + 12 - 4 + +9 - -7";
  s21::Calculation calculation8(input);
  EXPECT_EQ(15.0, calculation8.calcPolishNotaitonExp());

  input = "2 + +3";
  s21::Calculation calculation9(input);
  EXPECT_EQ(5.0, calculation9.calcPolishNotaitonExp());

  input = "2*10^-2 + 1";
  s21::Calculation calculation10(input);
  EXPECT_DOUBLE_EQ(1.02, calculation10.calcPolishNotaitonExp());
}

TEST(main_funcs, calculate_exceptions) {
  std::string input = "1.2.";
  s21::Calculation calculation3(input);
  EXPECT_ANY_THROW(calculation3.calcPolishNotaitonExp());

  input = "1 + .2";
  s21::Calculation calculation4(input);
  EXPECT_ANY_THROW(calculation4.calcPolishNotaitonExp());

  input = ". + 2";
  s21::Calculation calculation5(input);
  EXPECT_ANY_THROW(calculation5.calcPolishNotaitonExp());

  input = "2 + + 3";
  EXPECT_ANY_THROW(parseToPolishNotation(input));

  input = "2 / 0";
  s21::Calculation calculation6(input);
  EXPECT_ANY_THROW(calculation6.calcPolishNotaitonExp());

  input = "22sin2";
  s21::Calculation calculation10(input);
  EXPECT_ANY_THROW(calculation10.calcPolishNotaitonExp());

  input = "123nadnvvnda";
  EXPECT_ANY_THROW(parseToPolishNotation(input));

  input = "1 + 2 = 3";
  EXPECT_ANY_THROW(parseToPolishNotation(input));

  input = "mod2";
  s21::Calculation calculation11(input);
  EXPECT_ANY_THROW(calculation11.calcPolishNotaitonExp());

  input = "3*";
  s21::Calculation calculation12(input);
  EXPECT_ANY_THROW(calculation12.calcPolishNotaitonExp());

  input = "log(0)";
  s21::Calculation calculation13(input);
  EXPECT_ANY_THROW(calculation13.calcPolishNotaitonExp());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}