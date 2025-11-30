#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <queue>

bool isOperator(char c);
int priority(const std::string& op);
bool isFunction(const std::string& s);
double applyFunction(const std::string& func, double x);
double applyOperator(const std::string& op, double a, double b);
std::queue<std::string> toRPN(const std::string& expr);
double evalRPN(std::queue<std::string> rpn, double xvalue);

#endif