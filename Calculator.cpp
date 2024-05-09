#include <iostream>
#include <cmath>
#include <stack>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <stdexcept>

//Guido Fajardo
//Calculator with Basic Arithmetic, Trigonometry, Derivatives, and Integrals.

double applyOperation(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) throw std::runtime_error("Division by zero");
        return a / b;
    default: throw std::runtime_error("Unsupported operation");
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

// Additional functions for derivative and integral
double derivative(const std::string& func, double x) {
    const double h = 1e-5; // Step size
    double f_x = applyFunction(func, x);
    double f_xh = applyFunction(func, x + h);
    return (f_xh - f_x) / h;
}

double integral(const std::string& func, double a, double b, int n = 1000) {
    double step = (b - a) / n;
    double area = 0.0;
    for (int i = 0; i < n; i++) {
        double x1 = a + i * step;
        double x2 = a + (i + 1) * step;
        double y1 = applyFunction(func, x1);
        double y2 = applyFunction(func, x2);
        area += 0.5 * (y1 + y2) * step;
    }
    return area;
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
    std::cout << "Enter expression in postfix notation or type 'deriv' or 'integ':\n";
    std::getline(std::cin, input);

    if (input == "deriv" || input == "integ") {
        std::string function, interval;
        double x, a, b;
        if (input == "deriv") {
            std::cout << "Enter function and point (e.g., 'sin 3.14'):\n";
            std::getline(std::cin, function);
            x = std::stod(function.substr(function.find(' ') + 1));
            function = function.substr(0, function.find(' '));
            std::cout << "Derivative at " << x << " is " << derivative(function, x) << std::endl;
        } else {
            std::cout << "Enter function and interval [a, b] (e.g., 'cos 0 3.14'):\n";
            std::getline(std::cin, interval);
            std::istringstream iss(interval);
            iss >> function >> a >> b;
            std::cout << "Integral from " << a << " to " << b << " is " << integral(function, a, b) << std::endl;
        }
    } else {
        try {
            std::vector<std::string> tokens = tokenize(input);
            double result = evaluatePostfix(tokens);
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
