#include <iostream>
#include <cmath>
#include <stack>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <stdexcept>

//Guido Fajardo
//Calculator with Basic Arithmetic and trigonometry.

double applyOperation(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': 
        if (b == 0) throw std::runtime_error("Division by zero");
        return a / b;
    default:  throw std::runtime_error("Unsupported operation");
    }
}

double applyFunction(const std::string& func, double arg) {
    if (func == "log") return log(arg);
    if (func == "sin") return sin(arg);
    if (func == "cos") return cos(arg);
    if (func == "tan") return tan(arg);
    throw std::runtime_error("Unsupported function");
}

bool isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

bool isFunction(const std::string& token) {
    return token == "log" || token == "sin" || token == "cos" || token == "tan";
}

std::vector<std::string> tokenize(const std::string& s) {
    std::istringstream iss(s);
    std::istream_iterator<std::string> start(iss);
    std::istream_iterator<std::string> end;
    return std::vector<std::string>(start, end);
}


double evaluatePostfix(const std::vector<std::string>& tokens) {
    std::stack<double> stack;
    for (const std::string& token : tokens) {
        if (isOperator(token)) {
            if (stack.size() < 2) throw std::runtime_error("Invalid expression");
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            stack.push(applyOperation(a, b, token[0]));
        } else if (isFunction(token)) {
            if (stack.empty()) throw std::runtime_error("Invalid expression");
            double a = stack.top(); stack.pop();
            stack.push(applyFunction(token, a));
        } else {
            stack.push(std::stod(token));  // Explicit call to std::stod with proper syntax
        }
    }
    if (stack.size() != 1) throw std::runtime_error("Invalid expression");
    return stack.top();
}

int main() {
    std::string input;
    std::cout << "Enter expression in postfix notation (e.g., '3 4 + 2 * 7 / log'):\n";
    std::getline(std::cin, input);

    try {
        std::vector<std::string> tokens = tokenize(input);
        double result = evaluatePostfix(tokens);
        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
